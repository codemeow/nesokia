#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <string.h>

#include <nsk_util_meta.h>

#include "nsk_wav_segments.h"
#include "../../../arguments/nsk_args_options.h"
#include "../nsk_wav_midi.h"

/*!
 * Number of semitones per octave
 */
static const size_t semitones_peroctave = 12;

/*!
 * \brief  Local pitch estimate and scorer result.
 */
struct _segment_pitch {
    bool   found;
    int    midi;
    double frequency;
    double confidence;
    size_t support;
    double ratio;
    double duty;
    double score;
};

/*!
 * \brief  Time-domain pulse template fitting result.
 */
struct _segment_templatefit {
    bool   found;
    int    midi;
    double duty;
    double score;
    double residualratio;
    double phase;
    double gain;
    double dc;
};

/*!
 * \brief  One-frame pulse-width pitch evidence.
 */
struct _segment_pulsewidth {
    bool   found;
    int    midi;
    double confidence;
    size_t support;
    double duty;
    double templatescore;
    double templatemargin;
};

/*!
 * \brief  Context shared by edge-period decision rules.
 */
struct _segment_edge_context {
    size_t framecount;
    int    midimin;
};

/*!
 * \brief  Decision returned by an edge-period rule.
 */
struct _segment_edge_decision {
    bool decided;
    bool use;
    int  midi;
};

/*!
 * \brief  Edge-period decision rule callback.
 *
 * \param[in]  context    Segment-level decision context
 * \param[in]  spectral   Spectral pitch estimate
 * \param[in]  edge       Edge-period pitch estimate
 * \param[in]  threshold  Minimum confidence for regular edge usage
 * \param[out] decision   Rule decision
 * \return True if the rule completed successfully
 */
typedef bool (*_segment_edge_rule)(
    const struct _segment_edge_context *context,
    const struct _segment_pitch *spectral,
    const struct _segment_pitch *edge,
    double threshold,
    struct _segment_edge_decision *decision
);

/*!
 * \brief  NES pulse duty candidates, in percent.
 */
static const double _segment_duties[] = {
    12.5,
    25.0,
    50.0,
    75.0
};

/*!
 * \brief  Converts a frame index to the nearest sample index.
 *
 * \param[in]  wav    The wav being analyzed
 * \param[in]  frame  Grid frame index, fractional values allowed
 * \return Nearest sample index for the requested frame
 */
static size_t _segment_frame_sample(
    const struct nsk_wav *wav,
    double frame
) {
    return lround(
        frame *
        wav->format.samplerate /
        nsk_options_program.profile.boundary.grid.fps
    );
}

/*!
 * \brief  Computes the ratio of marks tagged as dense.
 *
 * \param[in]  wav  The wav being analyzed
 * \return Dense-mark ratio in the range 0..1
 */
static double _segment_dense_ratio(
    const struct nsk_wav *wav
) {
    if (wav->marks.count == 0) {
        return 0.0;
    }

    size_t dense = 0;

    for (size_t i = 0; i < wav->marks.count; i++) {
        if (wav->marks.list[i].indense) {
            dense++;
        }
    }

    return (double)dense / wav->marks.count;
}

/*!
 * \brief  Computes the ratio of accepted marks.
 *
 * \param[in]  wav  The wav being analyzed
 * \return Accepted-mark ratio in the range 0..1
 */
static double _segment_accepted_ratio(
    const struct nsk_wav *wav
) {
    if (wav->marks.count == 0) {
        return 0.0;
    }

    size_t accepted = 0;

    for (size_t i = 0; i < wav->marks.count; i++) {
        if (wav->marks.list[i].accepted) {
            accepted++;
        }
    }

    return (double)accepted / wav->marks.count;
}

/*!
 * \brief  Checks whether segment analysis should use dense-direct behavior.
 *
 * \param[in]  wav  The wav being analyzed
 * \return True if accepted marks are dense enough for dense-direct behavior
 */
static bool _segment_isdensedirect(
    const struct nsk_wav *wav
) {
    return
        _segment_accepted_ratio(wav) >=
        nsk_options_program.profile.segments.densedirectminacceptedratio;
}

/*!
 * \brief  Appends a segment spanning two frame indexes.
 *
 * \param[in,out]  wav         The wav whose segment list is extended
 * \param[in]      framestart  Start frame, inclusive
 * \param[in]      frameend    End frame, exclusive
 * \return True if the segment was appended successfully
 */
static bool _segment_appendframes(
    struct nsk_wav *wav,
    size_t          framestart,
    size_t          frameend
) {
    __typeof__((wav->segments.list)) ptr =
        realloc(
            wav->segments.list,
            (wav->segments.count + 1) * sizeof(wav->segments.list[0])
        );

    if (!ptr) {
        nsk_err(
            "Cannot reallocate the memory for the segments list"
        );
        return false;
    }

    wav->segments.list = ptr;
    wav->segments.list[wav->segments.count++] =
    (__typeof__(wav->segments.list[0])) {
        .framestart  = framestart,
        .frameend    = frameend,
        .samplestart = _segment_frame_sample(wav, framestart),
        .sampleend   = _segment_frame_sample(wav, frameend)
    };

    return true;
}

/*!
 * \brief  Appends a segment spanning two mark indexes.
 *
 * \param[in,out]  wav    The wav whose segment list is extended
 * \param[in]      start  Start mark index
 * \param[in]      end    End mark index
 * \return True if the segment was appended successfully
 */
static bool _segment_create(
    struct nsk_wav *wav,
    size_t          start,
    size_t          end
) {
    return _segment_appendframes(
        wav,
        wav->marks.list[start].frame,
        wav->marks.list[end].frame
    );
}

/*!
 * \brief  Creates the initial segment list from accepted marks.
 *
 * \param[in,out]  wav  The wav whose accepted marks are converted to segments
 * \return True if all initial segments were created successfully
 */
static bool _segments_create(
    struct nsk_wav      *wav
) {
    size_t acceptedstart = 0;

    for (size_t i = 0; i < wav->marks.count; i++) {
        if (!wav->marks.list[i].accepted) {
            continue;
        }

        if (!_segment_create(
            wav,
            acceptedstart,
            i
        )) {
            return false;
        }

        acceptedstart = i;
    }

    if (acceptedstart != wav->marks.count) {
        if (!_segment_appendframes(
            wav,
            wav->marks.list[acceptedstart].frame,
            wav->marks.count
        )) {
            return false;
        }
    }

    return true;
}

/*!
 * \brief  Computes peak-to-peak amplitude for one grid frame.
 *
 * \param[in]  wav    The wav being analyzed
 * \param[in]  frame  Grid frame index
 * \return Peak-to-peak amplitude inside the frame
 */
static double _frame_p2p(
    const struct nsk_wav *wav,
    size_t frame
) {
    const double fps = nsk_options_program.profile.boundary.grid.fps;
    const double samplerate = wav->format.samplerate;

    size_t start = lround((double)frame * samplerate / fps);
    size_t end   = lround((double)(frame + 1) * samplerate / fps);

    double min = wav->samples.raw.value[start];
    double max = wav->samples.raw.value[start];

    for (size_t i = start + 1; i < end; i++) {
        min = NSK_MIN(min, wav->samples.raw.value[i]);
        max = NSK_MAX(max, wav->samples.raw.value[i]);
    }

    return max - min;
}

/*!
 * \brief  Estimates the global segment silence threshold from frame p2p values.
 *
 * \param[in,out]  wav  The wav whose segment silence threshold is updated
 * \return True if the threshold was computed successfully
 */
static bool _segments_calcsilence(
    struct nsk_wav *wav
) {
    nsk_auto_free double *nonzero = calloc(
        wav->marks.count,
        sizeof(*nonzero)
    );
    if (!nonzero) {
        nsk_err("Cannot allocate memory for frame p2p values");
        return false;
    }

    size_t count = 0;

    for (size_t frame = 0; frame < wav->marks.count; frame++) {
        const double p2p = _frame_p2p(wav, frame);

        if (p2p > nsk_options_program.profile.segments.silencep2pfloor) {
            nonzero[count++] = p2p;
        }
    }

    if (count == 0) {
        wav->segments.silence =
            nsk_options_program.profile.segments.silencefallback;
        return true;
    }

    double quantile = 0.0;
    if (!nsk_math_quantiled(
        nsk_options_program.profile.segments.silencequantile,
        nonzero,
        count,
        &quantile
    )) {
        return false;
    }

    const double threshold =
        quantile * nsk_options_program.profile.segments.silencescale;

    wav->segments.silence = nsk_math_clampd(
        threshold,
        nsk_options_program.profile.segments.silencemin,
        nsk_options_program.profile.segments.silencemax
    );

    return true;
}


/*!
 * \brief  Updates the p2p amplitude of a segment.
 *
 * \param[in]      wav      The wav being analyzed
 * \param[in,out]  segment  Segment whose p2p field is updated
 * \return True if the value was updated successfully
 */
static bool _segment_p2p(
    const struct nsk_wav *wav,
    struct nsk_wav_segment *segment
) {
    double samplemin = wav->samples.raw.value[segment->samplestart];
    double samplemax = samplemin;

    for (size_t i = segment->samplestart + 1; i < segment->sampleend; i++) {
        samplemin = NSK_MIN(samplemin, wav->samples.raw.value[i]);
        samplemax = NSK_MAX(samplemax, wav->samples.raw.value[i]);
    }

    segment->p2p  = samplemax - samplemin;
    return true;
}

/*!
 * \brief  Updates the normalized segment volume estimate.
 *
 * \param[in]      wav      The wav being analyzed
 * \param[in,out]  segment  Segment whose volume field is updated
 * \return True if the value was updated successfully
 */
static bool _segment_volume(
    const struct nsk_wav *wav __attribute__((unused)),
    struct nsk_wav_segment *segment
) {
    if (!segment->active) {
        return true;
    }

    segment->volume = round(
        nsk_math_clampd(
            segment->p2p / 2,
            0.0,
            1.0
        ) * 100
    );
    return true;
}

/*!
 * \brief  Updates the active/rest state of a segment.
 *
 * \param[in]      wav      The wav holding the global silence threshold
 * \param[in,out]  segment  Segment whose active field is updated
 * \return True if the state was updated successfully
 */
static bool _segment_active(
    const struct nsk_wav *wav,
    struct nsk_wav_segment *segment
) {
    segment->active = segment->p2p >= wav->segments.silence;
    return true;
}

/*!
 * \brief  Computes real-FFT magnitudes for a segment analysis window.
 *
 * \param[in]   input       Input samples
 * \param[in]   inputcount  Number of input samples
 * \param[in]   fftsize     FFT size
 * \param[out]  magnitude   Magnitude output, fftsize / 2 + 1 items
 * \return True if the FFT magnitude was computed successfully
 */
static bool _segment_rfftmagnitude(
    const double *input,
    size_t inputcount,
    size_t fftsize,
    double *magnitude
) {
    nsk_auto_free double *real = calloc(fftsize / 2 + 1, sizeof(*real));
    nsk_auto_free double *imag = calloc(fftsize / 2 + 1, sizeof(*imag));

    if (!real || !imag) {
        nsk_err("Cannot allocate memory for segment FFT magnitude");
        return false;
    }

    if (
        !nsk_math_rfftd(
            input,
            inputcount,
            fftsize,
            real,
            imag
        )
    ) {
        return false;
    }

    for (size_t i = 0; i < fftsize / 2 + 1; i++) {
        magnitude[i] = hypot(real[i], imag[i]);
    }

    return true;
}

/*!
 * \brief  Samples the FFT magnitude spectrum at a fractional bin.
 *
 * \param[in]  magnitude  Magnitude spectrum
 * \param[in]  bincount   Number of magnitude bins
 * \param[in]  bin        Fractional bin position
 * \return Linearly interpolated magnitude, or 0 outside the spectrum
 */
static double _segment_spectralamplitude(
    const double *magnitude,
    size_t        bincount,
    double        bin
) {
    const size_t i = floor(bin);

    if (i + 1 >= bincount) {
        return 0.0;
    }

    const double frac = bin - i;

    return
        magnitude[i] * (1.0 - frac) +
        magnitude[i + 1] * frac;
}

/*!
 * \brief  Returns the smallest power of two greater than or equal to a value.
 *
 * \param[in]  x  Input value
 * \return Rounded power of two, or 0 on overflow
 */
static size_t _ceil_pow2(size_t x){
    if (x <= 1) {
        return 1;
    }

    if (x > (SIZE_MAX >> 1) + 1) {
        return 0;
    }

    x--;

    for (size_t shift = 1; shift < sizeof x * CHAR_BIT; shift <<= 1)
        x |= x >> shift;

    return x + 1;
}

/*!
 * \brief  Computes the trimmed sample window used for pitch analysis.
 *
 * \param[in]   wav          The wav being analyzed
 * \param[in]   segment      Segment whose analysis core is requested
 * \param[out]  samplestart  Trimmed start sample
 * \param[out]  sampleend    Trimmed end sample
 */
static void _segment_corebounds(
    const struct nsk_wav *wav,
    const struct nsk_wav_segment *segment,
    size_t *samplestart,
    size_t *sampleend
) {
    const uint32_t samplerate = wav->format.samplerate;
    const double fps = nsk_options_program.profile.boundary.grid.fps;
    const size_t framecount = segment->frameend - segment->framestart;

    double coreframestart = segment->framestart;
    double coreframeend   = segment->frameend;

    const size_t marginframes =
        nsk_options_program.profile.segments.marginframes;

    if (
        marginframes > 0 &&
        framecount >= marginframes * 2 + 1
    ) {
        coreframestart += marginframes;
        coreframeend   -= marginframes;
    }

    if (framecount > nsk_options_program.profile.segments.notrimshortframes) {
        const double duration =
            (coreframeend - coreframestart) /
            fps;
        const double ratiotrim =
            duration *
            NSK_MAX(0.0, nsk_options_program.profile.segments.trimratio);
        const double trim = NSK_MIN(
            NSK_MAX(0.0, nsk_options_program.profile.segments.trim),
            ratiotrim
        );
        const double core = duration - trim * 2.0;

        if (
            trim > 0.0 &&
            core >= nsk_options_program.profile.segments.mincore
        ) {
            const double trimframes =
                trim *
                fps;

            coreframestart += trimframes;
            coreframeend   -= trimframes;
        }
    }

    *samplestart = lround(coreframestart * samplerate / fps);
    *sampleend =
        lround(coreframeend * samplerate / fps);
}

/*!
 * \brief  Estimates pitch from stable same-polarity edge periods.
 *
 * \param[in]   wav          The wav being analyzed
 * \param[in]   samplestart  Start sample, inclusive
 * \param[in]   sampleend    End sample, exclusive
 * \param[out]  result       Edge-period pitch result
 * \return True if edge-period analysis completed successfully
 */
static bool _segment_edgeperiod(
    const struct nsk_wav *wav,
    size_t samplestart,
    size_t sampleend,
    struct _segment_pitch *result
) {
    *result = (struct _segment_pitch) {
        .found = false,
        .midi  = -1
    };

    const size_t samplecount = sampleend - samplestart;

    if (
        samplecount <
        nsk_options_program.profile.segments.edgeperiodminsamples
    ) {
        return true;
    }

    double peak = wav->samples.raw.value[samplestart];
    double trough = peak;

    for (size_t i = samplestart + 1; i < sampleend; i++) {
        peak = NSK_MAX(peak, wav->samples.raw.value[i]);
        trough = NSK_MIN(trough, wav->samples.raw.value[i]);
    }

    if (
        peak - trough <=
        nsk_options_program.profile.segments.edgeperiodp2pfloor
    ) {
        return true;
    }

    const double midline = (peak + trough) * 0.5;

    nsk_auto_free double *rising = calloc(samplecount, sizeof(*rising));
    nsk_auto_free double *falling = calloc(samplecount, sizeof(*falling));
    nsk_auto_free double *intervals = calloc(samplecount, sizeof(*intervals));

    if (!rising || !falling || !intervals) {
        nsk_err("Cannot allocate memory for segment edge periods");
        return false;
    }

    size_t risingcount = 0;
    size_t fallingcount = 0;

    double previous = wav->samples.raw.value[samplestart] - midline;

    for (size_t i = 1; i < samplecount; i++) {
        const double current =
            wav->samples.raw.value[samplestart + i] -
            midline;

        if (
            (previous <= 0.0 && current > 0.0) ||
            (previous < 0.0 && current >= 0.0)
        ) {
            const double denominator = current - previous;
            const double fraction =
                denominator == 0.0 ?
                0.0 :
                -previous / denominator;

            rising[risingcount++] =
                ((double)i - 1.0 + fraction) /
                wav->format.samplerate;

        } else if (
            (previous >= 0.0 && current < 0.0) ||
            (previous > 0.0 && current <= 0.0)
        ) {
            const double denominator = current - previous;
            const double fraction =
                denominator == 0.0 ?
                0.0 :
                -previous / denominator;

            falling[fallingcount++] =
                ((double)i - 1.0 + fraction) /
                wav->format.samplerate;
        }

        previous = current;
    }

    size_t intervalcount = 0;

    for (size_t i = 1; i < risingcount; i++) {
        intervals[intervalcount++] = rising[i] - rising[i - 1];
    }

    for (size_t i = 1; i < fallingcount; i++) {
        intervals[intervalcount++] = falling[i] - falling[i - 1];
    }

    if (intervalcount == 0) {
        return true;
    }

    double median = 0.0;

    if (!nsk_math_mediand(intervals, intervalcount, &median)) {
        return false;
    }

    if (median <= 0.0) {
        return true;
    }

    nsk_auto_free double *close = calloc(intervalcount, sizeof(*close));

    if (!close) {
        nsk_err("Cannot allocate memory for stable segment edge periods");
        return false;
    }

    size_t closecount = 0;

    for (size_t i = 0; i < intervalcount; i++) {
        if (
            fabs(intervals[i] - median) <=
            median *
            nsk_options_program.profile.segments.edgeperiodcloseperiodratio
        ) {
            close[closecount++] = intervals[i];
        }
    }

    const double *selected = intervals;
    size_t selectedcount = intervalcount;

    if (closecount >= 1) {
        selected = close;
        selectedcount = closecount;

        if (!nsk_math_mediand(selected, selectedcount, &median)) {
            return false;
        }

        if (median <= 0.0) {
            return true;
        }
    }

    double mean = 0.0;

    for (size_t i = 0; i < selectedcount; i++) {
        mean += selected[i];
    }

    mean /= selectedcount;

    double variance = 0.0;

    for (size_t i = 0; i < selectedcount; i++) {
        const double delta = selected[i] - mean;
        variance += delta * delta;
    }

    variance /= selectedcount;

    const double cv = sqrt(variance) / median;
    const double frequency = 1.0 / median;
    const int midi = nsk_midi_fromfreq(frequency);
    const double expected = nsk_midi_tofreq(midi);
    const double halfsemitone = pow(2.0, 1.0 / 24.0);
    const double freqlowest =
        nsk_options_program.profile.boundary.edgetrain.frequencylowest /
        halfsemitone;
    const double freqhighest =
        nsk_options_program.profile.boundary.edgetrain.frequencyhighest *
        halfsemitone;

    if (
        expected < freqlowest ||
        expected > freqhighest
    ) {
        return true;
    }

    const double cents = fabs(1200.0 * log2(frequency / expected));
    const double confidence =
        NSK_MAX(
            0.0,
            1.0 -
            cv *
            nsk_options_program.profile.segments.edgeperiodcvscale
        ) *
        NSK_MAX(
            0.0,
            1.0 -
            cents /
            nsk_options_program.profile.segments.edgeperiodcentsscale
        );

    *result = (struct _segment_pitch) {
        .found      = true,
        .midi       = midi,
        .frequency  = expected,
        .confidence = confidence,
        .support    = selectedcount
    };

    return true;
}

/*!
 * \brief  Creates an undecided edge-period decision.
 *
 * \return Undecided edge-period decision
 */
static struct _segment_edge_decision _segment_edge_nodecision(void) {
    return (struct _segment_edge_decision) {
        .decided = false
    };
}

/*!
 * \brief  Creates a decision that accepts edge-period pitch.
 *
 * \param[in]  midi  Accepted MIDI note
 * \return Decision that accepts the edge-period MIDI note
 */
static struct _segment_edge_decision _segment_edge_usedecision(int midi) {
    return (struct _segment_edge_decision) {
        .decided = true,
        .use     = true,
        .midi    = midi
    };
}

/*!
 * \brief  Creates a decision that blocks edge-period pitch.
 *
 * \param[in]  midi  Blocked MIDI note
 * \return Decision that blocks the edge-period MIDI note
 */
static struct _segment_edge_decision _segment_edge_blockdecision(int midi) {
    return (struct _segment_edge_decision) {
        .decided = true,
        .use     = false,
        .midi    = midi
    };
}

/*!
 * \brief  Uses edge-period pitch when spectral pitch is missing.
 *
 * \param[in]   context    Segment-level decision context
 * \param[in]   spectral   Spectral pitch estimate
 * \param[in]   edge       Edge-period pitch estimate
 * \param[in]   threshold  Minimum edge confidence
 * \param[out]  decision   Rule decision
 * \return True if the rule completed successfully
 */
static bool _segment_edge_rule_missing(
    const struct _segment_edge_context *context __attribute__((unused)),
    const struct _segment_pitch *spectral,
    const struct _segment_pitch *edge,
    double threshold,
    struct _segment_edge_decision *decision
) {
    *decision = _segment_edge_nodecision();

    if (spectral->found) {
        return true;
    }

    *decision =
        edge->confidence >= threshold ?
        _segment_edge_usedecision(edge->midi) :
        _segment_edge_blockdecision(edge->midi);

    return true;
}

/*!
 * \brief  Accepts edge-period pitch when it matches spectral pitch.
 *
 * \param[in]   context    Segment-level decision context
 * \param[in]   spectral   Spectral pitch estimate
 * \param[in]   edge       Edge-period pitch estimate
 * \param[in]   threshold  Minimum edge confidence
 * \param[out]  decision   Rule decision
 * \return True if the rule completed successfully
 */
static bool _segment_edge_rule_same(
    const struct _segment_edge_context *context __attribute__((unused)),
    const struct _segment_pitch *spectral,
    const struct _segment_pitch *edge,
    double threshold __attribute__((unused)),
    struct _segment_edge_decision *decision
) {
    *decision = _segment_edge_nodecision();

    if (!spectral->found || spectral->midi != edge->midi) {
        return true;
    }

    *decision = _segment_edge_usedecision(edge->midi);

    return true;
}

/*!
 * \brief  Allows high-confidence short-cycle edge correction.
 *
 * \param[in]   context    Segment-level decision context
 * \param[in]   spectral   Spectral pitch estimate
 * \param[in]   edge       Edge-period pitch estimate
 * \param[in]   threshold  Minimum edge confidence
 * \param[out]  decision   Rule decision
 * \return True if the rule completed successfully
 */
static bool _segment_edge_rule_cycle(
    const struct _segment_edge_context *context,
    const struct _segment_pitch *spectral,
    const struct _segment_pitch *edge,
    double threshold __attribute__((unused)),
    struct _segment_edge_decision *decision
) {
    *decision = _segment_edge_nodecision();

    if (!spectral->found || spectral->midi == edge->midi) {
        return true;
    }

    const int delta = edge->midi - spectral->midi;

    if (
        context->framecount <=
        nsk_options_program.profile.segments.cycleedgeoverridemaxframes &&
        (size_t)abs(delta) <=
        nsk_options_program.profile.segments.cycleedgeoverridemaxdeltast &&
        edge->support >=
        nsk_options_program.profile.segments.cycleedgeoverrideminsupport &&
        edge->confidence >=
        nsk_options_program.profile.segments.cycleedgeoverrideminconfidence
    ) {
        *decision = _segment_edge_usedecision(edge->midi);
    }

    return true;
}

/*!
 * \brief  Checks whether high 12.5 percent duty spectral evidence blocks edge correction.
 *
 * \param[in]  spectral  Spectral pitch estimate
 * \return True if the guard should block edge correction
 */
static bool _segment_edge_high125guard(
    const struct _segment_pitch *spectral
) {
    return
        spectral->found &&
        spectral->midi >=
        nsk_options_program.profile.segments.high125guardminmidi &&
        spectral->duty ==
        nsk_options_program.profile.segments.high125guardduty &&
        spectral->confidence >=
        nsk_options_program.profile.segments.high125guardminconfidence;
}

/*!
 * \brief  Allows guarded octave-down correction from edge periods.
 *
 * \param[in]   context    Segment-level decision context
 * \param[in]   spectral   Spectral pitch estimate
 * \param[in]   edge       Edge-period pitch estimate
 * \param[in]   threshold  Minimum edge confidence
 * \param[out]  decision   Rule decision
 * \return True if the rule completed successfully
 */
static bool _segment_edge_rule_octavedown(
    const struct _segment_edge_context *context,
    const struct _segment_pitch *spectral,
    const struct _segment_pitch *edge,
    double threshold __attribute__((unused)),
    struct _segment_edge_decision *decision
) {
    *decision = _segment_edge_nodecision();

    if (!spectral->found || spectral->midi == edge->midi) {
        return true;
    }

    if (_segment_edge_high125guard(spectral)) {
        return true;
    }

    if (
        edge->support <
        nsk_options_program.profile.segments.edgeperiodoctaveminsupport ||
        edge->confidence <
        nsk_options_program.profile.segments.edgeperiodoctaveminconfidence
    ) {
        return true;
    }

    const int delta = edge->midi - spectral->midi;

    if (delta == -(int)semitones_peroctave) {
        *decision = _segment_edge_usedecision(edge->midi);
        return true;
    }

    const int target = spectral->midi - (int)semitones_peroctave;

    if (
        target >= context->midimin &&
        abs(edge->midi - target) <= 1
    ) {
        *decision = _segment_edge_usedecision(target);
    }

    return true;
}

/*!
 * \brief  Applies the generic edge-period confidence threshold rule.
 *
 * \param[in]   context    Segment-level decision context
 * \param[in]   spectral   Spectral pitch estimate
 * \param[in]   edge       Edge-period pitch estimate
 * \param[in]   threshold  Minimum edge confidence
 * \param[out]  decision   Rule decision
 * \return True if the rule completed successfully
 */
static bool _segment_edge_rule_threshold(
    const struct _segment_edge_context *context __attribute__((unused)),
    const struct _segment_pitch *spectral,
    const struct _segment_pitch *edge,
    double threshold,
    struct _segment_edge_decision *decision
) {
    *decision = _segment_edge_nodecision();

    if (!spectral->found || spectral->midi == edge->midi) {
        return true;
    }

    const int delta = edge->midi - spectral->midi;
    const bool semitoneguard =
        abs(delta) == 1 &&
        spectral->confidence >=
        nsk_options_program.profile.segments.semitoneguardminconfidence &&
        edge->confidence < spectral->confidence;

    *decision =
        edge->confidence >= threshold && !semitoneguard ?
        _segment_edge_usedecision(edge->midi) :
        _segment_edge_blockdecision(edge->midi);

    return true;
}

/*!
 * \brief  Runs edge-period decision rules in priority order.
 *
 * \param[in]   context   Segment-level decision context
 * \param[in]   spectral  Spectral pitch estimate
 * \param[in]   edge      Edge-period pitch estimate
 * \param[out]  decision  Final edge-period decision
 * \return True if all required rules completed successfully
 */
static bool _segment_edgedecide(
    const struct _segment_edge_context *context,
    const struct _segment_pitch *spectral,
    const struct _segment_pitch *edge,
    struct _segment_edge_decision *decision
) {
    /*!
     * \brief  Ordered edge-period decision rule table.
     */
    static const struct {
        _segment_edge_rule rule;
    } table[] = {
        {
            .rule = _segment_edge_rule_missing
        },
        {
            .rule = _segment_edge_rule_same
        },
        {
            .rule = _segment_edge_rule_cycle
        },
        {
            .rule = _segment_edge_rule_octavedown
        },
        {
            .rule = _segment_edge_rule_threshold
        }
    };

    *decision = _segment_edge_nodecision();

    if (!edge->found || edge->support < 1) {
        return true;
    }

    const double threshold =
        context->framecount <=
        nsk_options_program.profile.segments.edgeperiodshortmaxframes ?
        nsk_options_program.profile.segments.edgeperiodshortminconfidence :
        nsk_options_program.profile.segments.edgeperiodminconfidence;

    for (size_t i = 0; i < NSK_SIZE(table); i++) {
        if (!table[i].rule(context, spectral, edge, threshold, decision)) {
            return false;
        }

        if (decision->decided) {
            return true;
        }
    }

    return true;
}

/*!
 * \brief  Estimates pitch with autocorrelation over MIDI-period lags.
 *
 * \param[in]   wav          The wav being analyzed
 * \param[in]   samplestart  Start sample, inclusive
 * \param[in]   sampleend    End sample, exclusive
 * \param[out]  pitch        Autocorrelation pitch result
 * \return True if period scoring completed successfully
 */
static bool _segment_period_note(
    const struct nsk_wav *wav,
    size_t samplestart,
    size_t sampleend,
    struct _segment_pitch *pitch
) {
    *pitch = (struct _segment_pitch) {
        .found = false,
        .midi  = -1
    };

    const size_t count = sampleend - samplestart;

    if (count < nsk_options_program.profile.segments.edgeperiodminsamples) {
        return true;
    }

    nsk_auto_free double *work = calloc(count, sizeof(*work));

    if (!work) {
        nsk_err("Cannot allocate memory for period note window");
        return false;
    }

    double mean = 0.0;

    for (size_t i = 0; i < count; i++) {
        mean += wav->samples.raw.value[samplestart + i];
    }

    mean /= count;

    double energy = 0.0;

    for (size_t i = 0; i < count; i++) {
        work[i] = wav->samples.raw.value[samplestart + i] - mean;
        energy += work[i] * work[i];
    }

    if (sqrt(energy) <= nsk_options_program.profile.segments.spectralfloor) {
        return true;
    }

    const double halfsemitone = pow(2.0, 1.0 / 24.0);
    const double freqlowest =
        nsk_options_program.profile.boundary.edgetrain.frequencylowest /
        halfsemitone;
    const double freqhighest =
        nsk_options_program.profile.boundary.edgetrain.frequencyhighest *
        halfsemitone;
    const int midimin = nsk_midi_fromfreq(freqlowest);
    const int midimax = nsk_midi_fromfreq(freqhighest);

    double bestscore = -1.0;
    int bestmidi = -1;

    for (int midi = midimin; midi <= midimax; midi++) {
        const double period = wav->format.samplerate / nsk_midi_tofreq(midi);
        const int baselag = lround(period);

        for (int lag = baselag - 1; lag <= baselag + 1; lag++) {
            if (lag <= 0 || (size_t)lag >= count) {
                continue;
            }

            double dot = 0.0;
            double leftnorm = 0.0;
            double rightnorm = 0.0;

            for (size_t i = 0; i + (size_t)lag < count; i++) {
                const double left = work[i];
                const double right = work[i + (size_t)lag];

                dot += left * right;
                leftnorm += left * left;
                rightnorm += right * right;
            }

            const double denom = sqrt(leftnorm) * sqrt(rightnorm);

            if (denom <= nsk_options_program.profile.segments.spectralfloor) {
                continue;
            }

            const double score = dot / denom;

            if (score > bestscore) {
                bestscore = score;
                bestmidi = midi;
            }
        }
    }

    if (
        bestmidi < 0 ||
        bestscore < nsk_options_program.profile.segments.periodminscore
    ) {
        return true;
    }

    *pitch = (struct _segment_pitch) {
        .found      = true,
        .midi       = bestmidi,
        .frequency  = nsk_midi_tofreq(bestmidi),
        .confidence = bestscore,
        .score      = bestscore
    };

    return true;
}

/*!
 * \brief  Scores one MIDI/duty pulse template against a sample window.
 *
 * \param[in]   wav          The wav being analyzed
 * \param[in]   samplestart  Start sample, inclusive
 * \param[in]   sampleend    End sample, exclusive
 * \param[in]   midi         MIDI note to test
 * \param[in]   duty         Duty value to test, in percent
 * \param[out]  fit          Template fit result
 * \return True if template scoring completed successfully
 */
static bool _segment_template_fit(
    const struct nsk_wav *wav,
    size_t samplestart,
    size_t sampleend,
    int midi,
    double duty,
    struct _segment_templatefit *fit
) {
    *fit = (struct _segment_templatefit) {
        .found = false,
        .midi  = midi,
        .duty  = duty,
        .score = -1.0,
        .residualratio = DBL_MAX
    };

    const size_t count = sampleend - samplestart;

    if (count < 4) {
        return true;
    }

    double mean = 0.0;

    for (size_t i = 0; i < count; i++) {
        mean += wav->samples.raw.value[samplestart + i];
    }

    mean /= count;

    double sst = 0.0;

    for (size_t i = 0; i < count; i++) {
        const double centered = wav->samples.raw.value[samplestart + i] - mean;
        sst += centered * centered;
    }

    if (sst <= nsk_options_program.profile.segments.spectralfloor) {
        fit->dc = mean;
        return true;
    }

    const double freq = nsk_midi_tofreq(midi);
    const double period = wav->format.samplerate / freq;
    size_t phasesteps = lround(period);

    phasesteps = NSK_MAX((size_t)1, phasesteps);
    phasesteps = NSK_MIN(
        phasesteps,
        nsk_options_program.profile.segments.templateresidualmaxphasesteps
    );

    const double dutyratio = duty / 100.0;
    struct _segment_templatefit best = *fit;

    for (size_t pi = 0; pi < phasesteps; pi++) {
        const double phase = (double)pi / phasesteps;
        double xmean = 0.0;

        for (size_t i = 0; i < count; i++) {
            const double t = (double)i / wav->format.samplerate;
            const double cycle = fmod(t * freq + phase, 1.0);
            const double x = cycle < dutyratio ? 1.0 : -1.0;

            xmean += x;
        }

        xmean /= count;

        double var = 0.0;
        double dot = 0.0;

        for (size_t i = 0; i < count; i++) {
            const double t = (double)i / wav->format.samplerate;
            const double cycle = fmod(t * freq + phase, 1.0);
            const double x = cycle < dutyratio ? 1.0 : -1.0;
            const double xc = x - xmean;
            const double yc =
                wav->samples.raw.value[samplestart + i] -
                mean;

            var += xc * xc;
            dot += xc * yc;
        }

        if (var <= nsk_options_program.profile.segments.spectralfloor) {
            continue;
        }

        const double gain = dot / var;
        const double dc = mean - gain * xmean;
        double sse = 0.0;

        for (size_t i = 0; i < count; i++) {
            const double t = (double)i / wav->format.samplerate;
            const double cycle = fmod(t * freq + phase, 1.0);
            const double x = cycle < dutyratio ? 1.0 : -1.0;
            const double residual =
                wav->samples.raw.value[samplestart + i] -
                (gain * x + dc);

            sse += residual * residual;
        }

        const double residualratio = sse / sst;
        const double score = 1.0 - residualratio;

        if (score > best.score) {
            best = (struct _segment_templatefit) {
                .found = true,
                .midi = midi,
                .duty = duty,
                .score = score,
                .residualratio = residualratio,
                .phase = phase,
                .gain = gain,
                .dc = dc
            };
        }
    }

    *fit = best;

    return true;
}

/*!
 * \brief  Finds the best pulse-template fit across MIDI and duty candidates.
 *
 * \param[in]   wav          The wav being analyzed
 * \param[in]   samplestart  Start sample, inclusive
 * \param[in]   sampleend    End sample, exclusive
 * \param[in]   midis        MIDI candidate list
 * \param[in]   midicount    Number of MIDI candidates
 * \param[out]  best         Best template fit
 * \return True if all candidate fits completed successfully
 */
static bool _segment_best_template_fit(
    const struct nsk_wav *wav,
    size_t samplestart,
    size_t sampleend,
    const int *midis,
    size_t midicount,
    struct _segment_templatefit *best
) {
    *best = (struct _segment_templatefit) {
        .found = false,
        .midi = -1,
        .duty = 0.0,
        .score = -1.0,
        .residualratio = DBL_MAX
    };

    for (size_t mi = 0; mi < midicount; mi++) {
        if (midis[mi] < 0) {
            continue;
        }

        for (size_t di = 0; di < NSK_SIZE(_segment_duties); di++) {
            struct _segment_templatefit fit = {0};

            if (!_segment_template_fit(
                wav,
                samplestart,
                sampleend,
                midis[mi],
                _segment_duties[di],
                &fit
            )) {
                return false;
            }

            if (fit.found && fit.score > best->score) {
                *best = fit;
            }
        }
    }

    return true;
}

/*!
 * \brief  Adds a MIDI candidate to a unique fixed-capacity candidate list.
 *
 * \param[in,out]  midis     Candidate list
 * \param[in,out]  count     Current candidate count
 * \param[in]      capacity  Maximum candidate count
 * \param[in]      midi      MIDI note to append
 * \return True if the candidate list remains valid
 */
static bool _segment_add_candidate_midi(
    int *midis,
    size_t *count,
    size_t capacity,
    int midi
) {
    if (midi < 0) {
        return true;
    }

    for (size_t i = 0; i < *count; i++) {
        if (midis[i] == midi) {
            return true;
        }
    }

    if (*count >= capacity) {
        nsk_err("Cannot append template MIDI candidate");
        return false;
    }

    midis[(*count)++] = midi;

    return true;
}

/*!
 * \brief  Builds MIDI candidates for residual template scoring.
 *
 * \param[in]   primary    Current segment MIDI, or negative if unknown
 * \param[in]   edge       Edge-period MIDI, or negative if unavailable
 * \param[in]   period     Autocorrelation MIDI, or negative if unavailable
 * \param[in]   framecount Segment length in frames
 * \param[out]  midis      Allocated MIDI candidate list
 * \param[out]  midicount  Number of MIDI candidates
 * \return True if the candidate list was built successfully
 */
static bool _segment_template_candidates(
    int primary,
    int edge,
    int period,
    size_t framecount,
    int **midis,
    size_t *midicount
) {
    const double halfsemitone = pow(2.0, 1.0 / 24.0);
    const int midimin = nsk_midi_fromfreq(
        nsk_options_program.profile.boundary.edgetrain.frequencylowest /
        halfsemitone
    );
    const int midimax = nsk_midi_fromfreq(
        nsk_options_program.profile.boundary.edgetrain.frequencyhighest *
        halfsemitone
    );
    const size_t capacity =
        framecount <=
        nsk_options_program.profile.segments.templateresidualfullsearchmaxframes ?
        (size_t)(midimax - midimin + 1) :
        15;

    *midis = calloc(capacity, sizeof(**midis));
    *midicount = 0;

    if (!*midis) {
        nsk_err("Cannot allocate memory for template MIDI candidates");
        return false;
    }

    if (
        framecount <=
        nsk_options_program.profile.segments.templateresidualfullsearchmaxframes
    ) {
        for (int midi = midimin; midi <= midimax; midi++) {
            if (!_segment_add_candidate_midi(
                *midis,
                midicount,
                capacity,
                midi
            )) {
                return false;
            }
        }

        return true;
    }

    /*!
     * \brief  Local MIDI offsets tested around primary evidence.
     */
    static const int deltas[] = {
        0,
        -1,
        1,
        -12,
        12
    };
    /*!
     * \brief  Primary pitch-evidence sources used as template search centers.
     */
    const int seeds[] = {
        primary,
        edge,
        period
    };

    for (size_t si = 0; si < NSK_SIZE(seeds); si++) {
        for (size_t di = 0; di < NSK_SIZE(deltas); di++) {
            const int midi = seeds[si] + deltas[di];

            if (midi < midimin || midi > midimax) {
                continue;
            }

            if (!_segment_add_candidate_midi(
                *midis,
                midicount,
                capacity,
                midi
            )) {
                return false;
            }
        }
    }

    return true;
}

/*!
 * \brief  Estimates one-frame low-note pitch from opposite-polarity pulse width.
 *
 * \param[in]   wav     The wav being analyzed
 * \param[in]   frame   Grid frame to analyze
 * \param[out]  result  Pulse-width pitch result
 * \return True if pulse-width scoring completed successfully
 */
static bool _segment_pulse_width_note_oneframe(
    const struct nsk_wav *wav,
    size_t frame,
    struct _segment_pulsewidth *result
) {
    *result = (struct _segment_pulsewidth) {
        .found = false,
        .midi  = -1,
        .templatescore = -1.0
    };

    const size_t samplestart = _segment_frame_sample(wav, frame);
    const size_t sampleend = _segment_frame_sample(wav, frame + 1);
    const size_t samplecount = sampleend - samplestart;
    const double freqlowest =
        nsk_options_program.profile.boundary.edgetrain.frequencylowest;
    const double freqhighest =
        nsk_options_program.profile.boundary.edgetrain.frequencyhighest;

    if (samplecount < nsk_options_program.profile.segments.edgeperiodminsamples) {
        return true;
    }

    double peak = wav->samples.raw.value[samplestart];
    double trough = peak;

    for (size_t i = samplestart + 1; i < sampleend; i++) {
        peak = NSK_MAX(peak, wav->samples.raw.value[i]);
        trough = NSK_MIN(trough, wav->samples.raw.value[i]);
    }

    if (
        peak - trough <=
        nsk_options_program.profile.segments.edgeperiodp2pfloor
    ) {
        return true;
    }

    struct {
        double time;
        int kind;
    } events[samplecount];
    size_t eventcount = 0;
    const double midline = (peak + trough) * 0.5;
    const double amplitude = (peak - trough) * 0.5;
    const double highthreshold =
        midline +
        amplitude *
        nsk_options_program.profile.boundary.schmitt.hysteresis;
    const double lowthreshold =
        midline -
        amplitude *
        nsk_options_program.profile.boundary.schmitt.hysteresis;
    int state = wav->samples.raw.value[samplestart] > midline ? 1 : -1;

    for (size_t i = 1; i < samplecount; i++) {
        const double previous = wav->samples.raw.value[samplestart + i - 1];
        const double current = wav->samples.raw.value[samplestart + i];
        int kind = 0;

        if (state <= 0 && current > highthreshold) {
            kind = 1;
            state = 1;

        } else if (state >= 0 && current < lowthreshold) {
            kind = -1;
            state = -1;
        }

        if (kind != 0) {
            const double denominator = current - previous;
            const double fraction =
                denominator == 0.0 ?
                0.0 :
                (midline - previous) / denominator;

            events[eventcount++] = (__typeof__(events[0])) {
                .time = ((double)i - 1.0 + fraction) /
                    wav->format.samplerate,
                .kind = kind
            };
        }
    }

    if (eventcount < 2) {
        return true;
    }

    struct {
        bool found;
        int midi;
        double edgescore;
        double duty;
    } candidates[eventcount * NSK_SIZE(_segment_duties)];
    size_t candidatecount = 0;

    for (size_t i = 1; i < eventcount; i++) {
        if (events[i - 1].kind == events[i].kind) {
            continue;
        }

        const double width = events[i].time - events[i - 1].time;

        if (width <= 0.0) {
            continue;
        }

        for (size_t di = 0; di < NSK_SIZE(_segment_duties); di++) {
            const double duty = _segment_duties[di];
            const double fraction =
                events[i - 1].kind > 0 ?
                duty / 100.0 :
                1.0 - duty / 100.0;

            if (fraction <= 0.0) {
                continue;
            }

            const double frequency = fraction / width;
            const int midi = nsk_midi_fromfreq(frequency);
            const double expected = nsk_midi_tofreq(midi);

            if (expected < freqlowest || expected > freqhighest) {
                continue;
            }

            const double cents = fabs(1200.0 * log2(frequency / expected));
            const double edgescore = NSK_MAX(
                0.0,
                1.0 -
                cents /
                nsk_options_program.profile.segments.pulsewidthcentsscale
            );
            bool seen = false;

            for (size_t ci = 0; ci < candidatecount; ci++) {
                if (candidates[ci].midi != midi) {
                    continue;
                }

                seen = true;

                if (edgescore > candidates[ci].edgescore) {
                    candidates[ci].edgescore = edgescore;
                    candidates[ci].duty = duty;
                }

                break;
            }

            if (!seen) {
                candidates[candidatecount++] = (__typeof__(candidates[0])) {
                    .found = true,
                    .midi = midi,
                    .edgescore = edgescore,
                    .duty = duty
                };
            }
        }
    }

    double bestcombined = -1.0;
    double secondcombined = 0.0;
    struct _segment_pulsewidth best = *result;

    for (size_t ci = 0; ci < candidatecount; ci++) {
        struct _segment_templatefit fit = {0};

        if (!_segment_template_fit(
            wav,
            samplestart,
            sampleend,
            candidates[ci].midi,
            candidates[ci].duty,
            &fit
        )) {
            return false;
        }

        const double combined =
            candidates[ci].edgescore *
            NSK_MAX(0.0, fit.score);

        if (combined > bestcombined) {
            secondcombined = bestcombined > 0.0 ? bestcombined : 0.0;
            bestcombined = combined;
            best = (struct _segment_pulsewidth) {
                .found = true,
                .midi = candidates[ci].midi,
                .confidence = combined,
                .support = 1,
                .duty = candidates[ci].duty,
                .templatescore = fit.score,
                .templatemargin = combined - secondcombined
            };

        } else if (combined > secondcombined) {
            secondcombined = combined;
            best.templatemargin = bestcombined - secondcombined;
        }
    }

    if (best.found) {
        best.templatemargin = bestcombined - secondcombined;
        *result = best;
    }

    return true;
}

/*!
 * \brief  Estimates folded duty class from high/low sample occupancy.
 *
 * \param[in]   wav      The wav being analyzed
 * \param[in]   segment  Segment whose waveform is measured
 * \param[out]  duty     Estimated folded duty, in percent
 * \return True if a folded duty class was estimated
 */
static bool _segment_estimate_folded_duty(
    const struct nsk_wav *wav,
    const struct nsk_wav_segment *segment,
    double *duty
) {
    *duty = 0.0;

    if (segment->sampleend - segment->samplestart < 4) {
        return false;
    }

    double peak = wav->samples.raw.value[segment->samplestart];
    double trough = peak;

    for (size_t i = segment->samplestart + 1; i < segment->sampleend; i++) {
        peak = NSK_MAX(peak, wav->samples.raw.value[i]);
        trough = NSK_MIN(trough, wav->samples.raw.value[i]);
    }

    if (peak - trough <= nsk_options_program.profile.segments.edgeperiodp2pfloor) {
        return false;
    }

    const double midline = (peak + trough) * 0.5;
    size_t highcount = 0;
    const size_t samplecount = segment->sampleend - segment->samplestart;

    for (size_t i = segment->samplestart; i < segment->sampleend; i++) {
        if (wav->samples.raw.value[i] > midline) {
            highcount++;
        }
    }

    const double highfraction = (double)highcount / samplecount;
    const double folded =
        NSK_MIN(highfraction, 1.0 - highfraction) *
        100.0;

    if (folded <= nsk_options_program.profile.segments.foldedduty12max) {
        *duty = 12.5;
        return true;
    }

    if (
        folded >= nsk_options_program.profile.segments.foldedduty25min &&
        folded <= nsk_options_program.profile.segments.foldedduty25max
    ) {
        *duty = 25.0;
        return true;
    }

    if (folded >= nsk_options_program.profile.segments.foldedduty50min) {
        *duty = 50.0;
        return true;
    }

    return false;
}

/*!
 * \brief  Resolves 25/75 percent duty polarity from high sample occupancy.
 *
 * \param[in]   wav      The wav being analyzed
 * \param[in]   segment  Segment whose waveform is measured
 * \param[out]  duty     Resolved duty, in percent
 * \return True if the high-sample ratio decisively selected 25 or 75 percent
 */
/*!
 * \brief  Applies risk-gated residual-template pitch and duty override.
 *
 * \param[in]      wav      The wav being analyzed
 * \param[in,out]  segment  Segment to refine
 * \param[out]     changed  Set to true when the segment is modified
 * \return True if template override completed successfully
 */
static bool _segment_template_override(
    const struct nsk_wav *wav,
    struct nsk_wav_segment *segment,
    bool *changed
) {
    if (
        !nsk_options_program.profile.segments.templateresidualenabled ||
        !segment->active
    ) {
        return true;
    }

    const size_t framecount = segment->frameend - segment->framestart;

    if (
        segment->midi >= 0 &&
        framecount >
        nsk_options_program.profile.segments.edgeperiodshortmaxframes &&
        segment->confidence >=
        nsk_options_program.profile.segments
            .templateresidualprefilterminconfidence
    ) {
        return true;
    }

    size_t samplestart = 0;
    size_t sampleend = 0;

    _segment_corebounds(wav, segment, &samplestart, &sampleend);

    struct _segment_pitch edge = {0};
    struct _segment_pitch period = {0};

    if (!_segment_edgeperiod(wav, samplestart, sampleend, &edge)) {
        return false;
    }

    if (!_segment_period_note(wav, samplestart, sampleend, &period)) {
        return false;
    }

    bool risk = segment->midi < 0;
    /*!
     * \brief  Independent pitch estimates used to decide if template scoring is needed.
     */
    const int comparisons[] = {
        edge.found ? edge.midi : -1,
        period.found ? period.midi : -1
    };

    for (size_t i = 0; i < NSK_SIZE(comparisons); i++) {
        if (comparisons[i] < 0) {
            continue;
        }

        if (segment->midi < 0) {
            risk = true;
            continue;
        }

        const int delta = abs(comparisons[i] - segment->midi);

        if (
            delta == 1 ||
            delta == (int)semitones_peroctave ||
            delta == (int)semitones_peroctave * 2 ||
            (
                framecount <=
                nsk_options_program.profile.segments.edgeperiodshortmaxframes &&
                NSK_MIN(comparisons[i], segment->midi) <=
                nsk_options_program.profile.segments
                    .templateresiduallowshortmaxmidi
            )
        ) {
            risk = true;
            break;
        }
    }

    if (
        segment->midi >= 0 &&
        framecount <=
        nsk_options_program.profile.segments.edgeperiodshortmaxframes &&
        segment->midi <=
        nsk_options_program.profile.segments.templateresiduallowshortmaxmidi
    ) {
        risk = true;
    }

    if (!risk) {
        return true;
    }

    nsk_auto_free int *midis = NULL;
    size_t midicount = 0;

    if (!_segment_template_candidates(
        segment->midi,
        edge.found ? edge.midi : -1,
        period.found ? period.midi : -1,
        framecount,
        &midis,
        &midicount
    )) {
        return false;
    }

    if (midicount == 0) {
        return true;
    }

    struct _segment_templatefit best = {0};

    if (!_segment_best_template_fit(
        wav,
        samplestart,
        sampleend,
        midis,
        midicount,
        &best
    )) {
        return false;
    }

    if (
        !best.found ||
        best.score <
        nsk_options_program.profile.segments.templateresidualminscore
    ) {
        return true;
    }

    double currentscore = -1.0;

    if (segment->midi >= 0) {
        struct _segment_templatefit current = {0};
        int currentmidi[] = {
            segment->midi
        };

        if (!_segment_best_template_fit(
            wav,
            samplestart,
            sampleend,
            currentmidi,
            NSK_SIZE(currentmidi),
            &current
        )) {
            return false;
        }

        currentscore = current.score;

        if (best.midi == segment->midi) {
            if (
                best.score > segment->confidence ||
                fabs(best.duty - segment->duty) >
                nsk_options_program.profile.segments.mergedutyequalitythreshold
            ) {
                segment->confidence = NSK_MAX(segment->confidence, best.score);
                segment->duty = best.duty;
                *changed = true;
            }

            return true;
        }
    }

    const double improvement = best.score - currentscore;
    double threshold =
        nsk_options_program.profile.segments.templateresidualminimprovement;

    if (segment->midi >= 0) {
        const int delta = abs(best.midi - segment->midi);
        const bool lowshort =
            framecount <=
            nsk_options_program.profile.segments.edgeperiodshortmaxframes &&
            NSK_MIN(best.midi, segment->midi) <=
            nsk_options_program.profile.segments
                .templateresiduallowshortmaxmidi;

        if (
            delta == (int)semitones_peroctave ||
            delta == (int)semitones_peroctave * 2
        ) {
            threshold =
                nsk_options_program.profile.segments
                    .templateresidualoctaveminimprovement;

        } else if (delta == 1) {
            threshold =
                nsk_options_program.profile.segments
                    .templateresidualsemitoneminimprovement;

        } else if (lowshort) {
            threshold =
                nsk_options_program.profile.segments
                    .templateresiduallowshortminimprovement;
        }

    }

    if (improvement < threshold) {
        return true;
    }

    segment->midi = best.midi;
    segment->frequency = nsk_midi_tofreq(best.midi);
    segment->confidence = NSK_MAX(segment->confidence, best.score);
    segment->duty = best.duty;
    *changed = true;

    return true;
}

/*!
 * \brief  Classifies a sample window using spectral, edge, and guard evidence.
 *
 * \param[in]   wav              The wav being analyzed
 * \param[in]   coresamplestart  Start sample of the analysis window
 * \param[in]   coresampleend    End sample of the analysis window
 * \param[in]   framecount       Source segment length in frames
 * \param[out]  pitch            Final pitch estimate for the window
 * \return True if the window was analyzed successfully
 */
static bool _segment_analyze_window(
    const struct nsk_wav *wav,
    size_t coresamplestart,
    size_t coresampleend,
    size_t framecount,
    struct _segment_pitch *pitch
) {
    *pitch = (struct _segment_pitch) {
        .found = false,
        .midi  = -1
    };

    const size_t samplecount = coresampleend - coresamplestart;
    const uint32_t samplerate = wav->format.samplerate;

    nsk_auto_free double *fftinput = calloc(
        samplecount,
        sizeof(wav->samples.raw.value[0])
    );
    if (!fftinput) {
        nsk_err("Cannot allocate memory for FFT input values");
        return false;
    }

    double sum = 0;

    for (size_t i = coresamplestart; i < coresampleend; i++) {
        sum += wav->samples.raw.value[i];
    }

    const double mean = sum / samplecount;

    for (size_t i = 0; i < samplecount; i++) {
        double window;

        if (samplecount > nsk_options_program.profile.segments.hannwindowthreshold) {
            const double hann = nsk_math_window_hannd(
                i,
                samplecount
            );
            window = sqrt(
                NSK_MAX(
                    hann,
                    nsk_options_program.profile.segments.hannwindowlimit
                )
            );

        } else {
            window = 1.0;
        }

        fftinput[i] =
            (wav->samples.raw.value[coresamplestart + i] - mean) * window;
    }

    size_t basefftsize = nsk_options_program.profile.segments.fftsize;

    if (
        _segment_isdensedirect(wav) &&
        basefftsize > nsk_options_program.profile.segments.densedirectfftsize
    ) {
        basefftsize = nsk_options_program.profile.segments.densedirectfftsize;
    }

    if (
        framecount <=
        nsk_options_program.profile.segments.shortfftmaxframes &&
        samplerate >=
        nsk_options_program.profile.segments.shortfftminsamplerate
    ) {
        basefftsize = NSK_MAX(
            basefftsize,
            nsk_options_program.profile.segments.shortfftsize
        );
    }

    size_t fftsize = NSK_MAX(
        basefftsize,
        _ceil_pow2(NSK_MAX(8, samplecount))
    );

    const size_t bincount = fftsize / 2 + 1;
    nsk_auto_free double *magnitude = calloc(bincount, sizeof(*magnitude));
    if (!magnitude) {
        nsk_err("Cannot allocate memory for segment magnitude values");
        return false;
    }

    if (!_segment_rfftmagnitude(
        fftinput,
        samplecount,
        fftsize,
        magnitude
    )) {
        return false;
    }

    double total = 0.0;

    for (size_t i = 1; i < bincount; i++) {
        total += magnitude[i] * magnitude[i];
    }

    total = sqrt(total);

    const double halfsemitone = pow(2.0, 1.0 / 24.0);
    const double freqlowest = nsk_options_program.profile.boundary.edgetrain.frequencylowest / halfsemitone;
    const double freqhighest = nsk_options_program.profile.boundary.edgetrain.frequencyhighest * halfsemitone;

    const int midimin = nsk_midi_fromfreq(freqlowest);
    const int midimax = nsk_midi_fromfreq(freqhighest);

    const size_t midicount = midimax - midimin + 1;
    nsk_auto_free struct _segment_pitch *scores = calloc(
        midicount,
        sizeof(*scores)
    );
    if (!scores) {
        nsk_err("Cannot allocate memory for segment MIDI scores");
        return false;
    }

    struct _segment_pitch best = {
        .found = false,
        .midi  = -1
    };

    for (int midi = midimin; midi <= midimax; midi++) {
        double freq = nsk_midi_tofreq(midi);

        if (freq < freqlowest || freq > freqhighest) {
            continue;
        }

        const double nyquist = wav->format.samplerate * 0.5;
        const size_t hcount = NSK_MIN(
            nsk_options_program.profile.segments.maxharmonics,
            floor(nyquist / freq)
        );

        if (hcount < nsk_options_program.profile.segments.minharmonics) {
            continue;
        }

        double amps[hcount];

        for (size_t harmonic = 1; harmonic <= hcount; harmonic++) {
            const double harmonicfreq = freq * harmonic;
            const double bin =
                harmonicfreq * fftsize / wav->format.samplerate;

            amps[harmonic - 1] = _segment_spectralamplitude(
                magnitude,
                bincount,
                bin
            );
        }

        double norm = 0.0;
        double ampmax = 0.0;

        for (size_t i = 0; i < hcount; i++) {
            norm += amps[i] * amps[i];
            ampmax = NSK_MAX(ampmax, amps[i]);
        }

        norm = sqrt(norm);

        if (norm <= nsk_options_program.profile.segments.spectralfloor) {
            continue;
        }

        /*!
         * \brief  Harmonic pulse duty candidates, as cycle fractions.
         */
        static const double duties[] = {
            0.125,
            0.250,
            0.500,
            0.750
        };

        double bestscore = -1.0;
        double bestduty = 0.0;

        for (size_t di = 0; di < NSK_SIZE(duties); di++) {
            const double duty = duties[di];
            double templatenorm = 0.0;

            for (size_t harmonic = 1; harmonic <= hcount; harmonic++) {
                const double value = fabs(
                    sin(M_PI * harmonic * duty) / harmonic
                );

                templatenorm += value * value;
            }

            templatenorm = sqrt(templatenorm);

            if (
                templatenorm <=
                nsk_options_program.profile.segments.spectralfloor
            ) {
                continue;
            }

            double score = 0.0;

            for (size_t harmonic = 1; harmonic <= hcount; harmonic++) {
                const double templatevalue = fabs(
                    sin(M_PI * harmonic * duty) / harmonic
                );

                score +=
                    (amps[harmonic - 1] / norm) *
                    (templatevalue / templatenorm);
            }

            if (score > bestscore) {
                bestscore = score;
                bestduty = duty * 100.0;
            }
        }

        const double fundamental = amps[0] / ampmax;
        const double shaped =
            bestscore *
            (
                nsk_options_program.profile.segments.scorefundamentalbase +
                nsk_options_program.profile.segments.scorefundamentalweight *
                NSK_MIN(1.0, fundamental)
            );
        const double ratio = NSK_MIN(1.0, norm / total);
        const double confidence =
            shaped *
            (
                nsk_options_program.profile.segments.confidencebase +
                nsk_options_program.profile.segments.confidenceratioweight *
                NSK_MIN(
                    1.0,
                    ratio /
                    nsk_options_program.profile.segments.confidenceratioscale
                )
            );

        struct _segment_pitch item = {
            .found      = true,
            .midi       = midi,
            .frequency  = freq,
            .confidence = confidence,
            .support    = 0,
            .ratio      = ratio,
            .duty       = bestduty,
            .score      = shaped
        };

        scores[midi - midimin] = item;

        if (!best.found || confidence > best.confidence) {
            best = item;
        }
    }

    /*!
     * \brief  Lower-octave candidates tested by the octave guard.
     */
    const struct {
        int shift;
    } octaveguard[] = {
        {
            .shift = (int)semitones_peroctave * -1
        },
        {
            .shift = (int)semitones_peroctave * -2
        }
    };

    for (size_t i = 0; best.found && i < NSK_SIZE(octaveguard); i++) {
        const int candidate = best.midi + octaveguard[i].shift;

        if (candidate < midimin || candidate > midimax) {
            continue;
        }

        const struct _segment_pitch item = scores[candidate - midimin];

        if (
            item.found &&
            item.score >=
            best.score -
            nsk_options_program.profile.segments.octaveguardscoremargin &&
            item.ratio >=
            best.ratio *
            nsk_options_program.profile.segments.octaveguardratiomultiplier
        ) {
            best = item;
            break;
        }
    }

    struct _segment_pitch spectral = best;

    if (
        !spectral.found ||
        spectral.confidence <
        nsk_options_program.profile.segments.minconfidence ||
        spectral.ratio <
        nsk_options_program.profile.segments.minharmonicratio
    ) {
        spectral.found = false;
        spectral.midi  = -1;
    }

    struct _segment_pitch edge = {0};

    if (!_segment_edgeperiod(
        wav,
        coresamplestart,
        coresampleend,
        &edge
    )) {
        return false;
    }

    const struct _segment_edge_context edgecontext = {
        .framecount = framecount,
        .midimin    = midimin
    };
    struct _segment_edge_decision edgedecision = {0};

    if (!_segment_edgedecide(
        &edgecontext,
        &spectral,
        &edge,
        &edgedecision
    )) {
        return false;
    }

    if (edgedecision.use) {
        *pitch = (struct _segment_pitch) {
            .found      = true,
            .midi       = edgedecision.midi,
            .frequency  = nsk_midi_tofreq(edgedecision.midi),
            .confidence =
            spectral.found ?
            NSK_MAX(spectral.confidence, edge.confidence) :
            edge.confidence,
            .ratio      = best.found ? best.ratio : 0.0,
            .duty       = best.found ? best.duty : 0.0,
            .score      = best.found ? best.score : 0.0
        };

        return true;
    }

    if (!spectral.found) {
        *pitch = (struct _segment_pitch) {
            .found      = false,
            .midi       = -1,
            .confidence = best.found ? best.confidence : 0.0,
            .ratio      = best.found ? best.ratio : 0.0,
            .duty       = best.found ? best.duty : 0.0,
            .score      = best.found ? best.score : 0.0
        };

        return true;
    }

    *pitch = spectral;

    return true;
}

/*!
 * \brief  Updates segment pitch fields from its analysis window.
 *
 * \param[in]      wav      The wav being analyzed
 * \param[in,out]  segment  Segment whose pitch fields are updated
 * \return True if classification completed successfully
 */
static bool _segment_classify(
    const struct nsk_wav *wav,
    struct nsk_wav_segment *segment
) {
    if (!segment->active) {
        segment->midi = -1;
        return true;
    }

    size_t coresamplestart = 0;
    size_t coresampleend = 0;

    _segment_corebounds(
        wav,
        segment,
        &coresamplestart,
        &coresampleend
    );

    struct _segment_pitch pitch = {0};

    if (!_segment_analyze_window(
        wav,
        coresamplestart,
        coresampleend,
        segment->frameend - segment->framestart,
        &pitch
    )) {
        return false;
    }

    segment->midi          = pitch.midi;
    segment->frequency     = pitch.found ? pitch.frequency : 0.0;
    segment->confidence    = pitch.confidence;
    segment->harmonicratio = pitch.ratio;
    segment->duty          = pitch.duty;

    return true;
}

/*!
 * \brief  Recomputes all primary measurements for a segment.
 *
 * \param[in]      wav      The wav being analyzed
 * \param[in,out]  segment  Segment to recompute
 * \return True if all primary measurements were recomputed successfully
 */
static bool _segment_recalc(
    const struct nsk_wav *wav,
    struct nsk_wav_segment *segment
) {
    if (!_segment_p2p(wav, segment)) {
        return false;
    }

    if (!_segment_active(wav, segment)) {
        return false;
    }

    if (!_segment_volume(wav, segment)) {
        return false;
    }

    if (!_segment_classify(wav, segment)) {
        return false;
    }

    return true;
}

/*!
 * \brief  Creates an unclassified segment for a frame interval.
 *
 * \param[in]  wav         The wav being analyzed
 * \param[in]  framestart  Start frame, inclusive
 * \param[in]  frameend    End frame, exclusive
 * \return Segment initialized with frame and sample bounds
 */
static struct nsk_wav_segment _segment_fromframes(
    const struct nsk_wav *wav,
    size_t framestart,
    size_t frameend
) {
    const double samplerate = wav->format.samplerate;
    const double fps = nsk_options_program.profile.boundary.grid.fps;

    return (struct nsk_wav_segment) {
        .framestart  = framestart,
        .frameend    = frameend,
        .samplestart = lround((double)framestart * samplerate / fps),
        .sampleend   = lround((double)frameend * samplerate / fps),
        .midi        = -1
    };
}

/*!
 * \brief  Replaces a contiguous segment range with a prepared item list.
 *
 * \param[in,out]  wav          The wav whose segment list is modified
 * \param[in]      index        First segment index to replace
 * \param[in]      removecount  Number of existing segments to remove
 * \param[in]      items        Replacement segment list
 * \param[in]      itemcount    Replacement segment count
 * \return True if the replacement completed successfully
 */
static bool _segments_replace_range(
    struct nsk_wav *wav,
    size_t index,
    size_t removecount,
    const struct nsk_wav_segment *items,
    size_t itemcount
) {
    const size_t newcount = wav->segments.count - removecount + itemcount;
    __typeof__(wav->segments.list) list = NULL;

    if (newcount > 0) {
        list = malloc(newcount * sizeof(*list));

        if (!list) {
            nsk_err("Cannot allocate memory for segment replacement");
            return false;
        }

        memcpy(
            list,
            wav->segments.list,
            index * sizeof(*list)
        );

        if (itemcount > 0) {
            memcpy(
                &list[index],
                items,
                itemcount * sizeof(*list)
            );
        }

        memcpy(
            &list[index + itemcount],
            &wav->segments.list[index + removecount],
            (wav->segments.count - index - removecount) * sizeof(*list)
        );
    }

    free(wav->segments.list);
    wav->segments.list = list;
    wav->segments.count = newcount;

    return true;
}

/*!
 * \brief  Checks whether two neighboring segments may be merged.
 *
 * \param[in]  left   Left segment
 * \param[in]  right  Right segment
 * \return True if both segments have the same merge identity
 */
static bool _segments_isequal(
    const struct nsk_wav_segment *left,
    const struct nsk_wav_segment *right
);

/*!
 * \brief  Computes p2p discontinuity between adjacent grid frames.
 *
 * \param[in]  wav    The wav being analyzed
 * \param[in]  frame  Right frame of the boundary
 * \return Absolute p2p delta, or DBL_MAX for an invalid boundary
 */
static double _segment_mark_p2p_delta(
    const struct nsk_wav *wav,
    size_t frame
) {
    if (frame == 0 || frame >= wav->marks.count) {
        return DBL_MAX;
    }

    return fabs(_frame_p2p(wav, frame) - _frame_p2p(wav, frame - 1));
}

/*!
 * \brief  Checks segment identity using active state, MIDI, volume, and duty.
 *
 * \param[in]  left   Left segment
 * \param[in]  right  Right segment
 * \return True if both segments have the same merge identity
 */
static bool _segments_isequal(
    const struct nsk_wav_segment *left,
    const struct nsk_wav_segment *right
) {
    if (left->active != right->active) {
        return false;
    }

    if (!left->active) {
        return true;
    }

    if (left->midi != right->midi) {
        return false;
    }

    if (
        fabs(left->volume - right->volume) >
        nsk_options_program.profile.segments.mergevolumeequalitythreshold
    ) {
        return false;
    }

    if (
        fabs(left->duty - right->duty) >
        nsk_options_program.profile.segments.mergedutyequalitythreshold
    ) {
        return false;
    }

    return true;
}

/*!
 * \brief  Computes a weighted average for merged segment measurements.
 *
 * \param[in]  left         Left value
 * \param[in]  leftweight   Left value weight
 * \param[in]  right        Right value
 * \param[in]  rightweight  Right value weight
 * \return Weighted average, or 0 when the total weight is zero
 */
static double _segment_weightedaverage(
    double left,
    size_t leftweight,
    double right,
    size_t rightweight
) {
    const size_t totalweight = leftweight + rightweight;

    if (totalweight == 0) {
        return 0.0;
    }

    return
        (
            left * leftweight +
            right * rightweight
        ) /
        totalweight;
}

/*!
 * \brief  Merges the right segment measurements into the left segment.
 *
 * \param[in,out]  left   Segment that receives merged measurements
 * \param[in]      right  Segment being merged into left
 */
static void _segments_mergeitem(
    struct nsk_wav_segment *left,
    const struct nsk_wav_segment *right
) {
    const size_t leftframes = left->frameend - left->framestart;
    const size_t rightframes = right->frameend - right->framestart;

    left->frameend = right->frameend;
    left->sampleend = right->sampleend;
    left->p2p = NSK_MAX(left->p2p, right->p2p);
    left->volume = _segment_weightedaverage(
        left->volume,
        leftframes,
        right->volume,
        rightframes
    );
    left->confidence = NSK_MIN(left->confidence, right->confidence);
    left->harmonicratio = NSK_MIN(left->harmonicratio, right->harmonicratio);
    left->duty = _segment_weightedaverage(
        left->duty,
        leftframes,
        right->duty,
        rightframes
    );
}

/*!
 * \brief  Compacts adjacent equal segments in place.
 *
 * \param[in,out]  wav  The wav whose segment list is compacted
 * \return True if compaction completed successfully
 */
static bool _segments_merge(
    struct nsk_wav *wav
) {
    if (wav->segments.count < 2) {
        return true;
    }

    bool *merged = calloc(
        wav->segments.count,
        sizeof(*merged)
    );
    if (!merged) {
        nsk_err("Cannot allocate memory for merged segment flags");
        return false;
    }

    size_t write = 0;

    for (size_t read = 1; read < wav->segments.count; read++) {
        if (_segments_isequal(
            &wav->segments.list[write],
            &wav->segments.list[read]
        )) {
            _segments_mergeitem(
                &wav->segments.list[write],
                &wav->segments.list[read]
            );
            merged[write] = true;
            continue;
        }

        write++;

        if (write != read) {
            wav->segments.list[write] = wav->segments.list[read];
        }

        merged[write] = false;
    }

    wav->segments.count = write + 1;

    for (size_t i = 0; i < wav->segments.count; i++) {
        if (!merged[i]) {
            continue;
        }

        if (!_segment_recalc(wav, &wav->segments.list[i])) {
            free(merged);
            return false;
        }

        bool ignored = false;

        if (!_segment_template_override(wav, &wav->segments.list[i], &ignored)) {
            free(merged);
            return false;
        }
    }

    free(merged);

    return true;
}

/*!
 * \brief  Checks whether an internal candidate mark is useful as a segment split.
 *
 * \param[in]   wav      The wav being analyzed
 * \param[in]   segment  Segment that contains the candidate mark
 * \param[in]   frame    Candidate split frame
 * \param[out]  left     Reclassified left side
 * \param[out]  right    Reclassified right side
 * \param[out]  use      Set to true if the mark should split the segment
 * \return True if the split check completed successfully
 */
static bool _segments_candidate_split(
    const struct nsk_wav *wav,
    const struct nsk_wav_segment *segment,
    size_t frame,
    struct nsk_wav_segment *left,
    struct nsk_wav_segment *right,
    bool *use
) {
    *use = false;

    if (
        frame <= segment->framestart ||
        frame >= segment->frameend ||
        segment->frameend - segment->framestart >
        nsk_options_program.profile.segments.candidatesplitmaxframes ||
        frame >= wav->marks.count ||
        wav->marks.list[frame].count == 0
    ) {
        return true;
    }

    if (
        segment->frameend - segment->framestart <=
        nsk_options_program.profile.segments.framegridsplitmaxframes &&
        !wav->marks.list[frame].indense
    ) {
        return true;
    }

    if (
        !wav->marks.list[frame].indense &&
        wav->marks.list[frame].maxabsdeltast <
        nsk_options_program.profile.segments.candidatesplitmindeltast
    ) {
        return true;
    }

    *left = _segment_fromframes(
        wav,
        segment->framestart,
        frame
    );
    *right = _segment_fromframes(
        wav,
        frame,
        segment->frameend
    );

    if (!_segment_recalc(wav, left)) {
        return false;
    }

    if (!_segment_recalc(wav, right)) {
        return false;
    }

    bool ignored = false;

    if (!_segment_template_override(wav, left, &ignored)) {
        return false;
    }

    if (!_segment_template_override(wav, right, &ignored)) {
        return false;
    }

    *use =
        left->active &&
        right->active &&
        left->midi >= 0 &&
        right->midi >= 0 &&
        left->midi != right->midi &&
        left->confidence >=
        nsk_options_program.profile.segments.edgeconfirmedsplitminstateconfidence &&
        right->confidence >=
        nsk_options_program.profile.segments.edgeconfirmedsplitminstateconfidence;

    return true;
}

/*!
 * \brief  Splits segments at strong internal candidate marks.
 *
 * \param[in,out]  wav      The wav whose segments are refined
 * \param[out]     changed  Set to true when the segment list is modified
 * \return True if the refiner completed successfully
 */
static bool _segments_split_candidate_marks(
    struct nsk_wav *wav,
    bool *changed
) {
    *changed = false;

    if (_segment_isdensedirect(wav)) {
        return true;
    }

    if (
        wav->segments.count >
        nsk_options_program.profile.segments.candidatesplitmaxsegments
    ) {
        return true;
    }

    nsk_auto_free struct nsk_wav_segment *items = calloc(
        wav->segments.count + wav->marks.count,
        sizeof(*items)
    );

    if (!items) {
        nsk_err("Cannot allocate memory for candidate mark segment split");
        return false;
    }

    size_t itemcount = 0;

    for (size_t i = 0; i < wav->segments.count; i++) {
        struct nsk_wav_segment current = wav->segments.list[i];

        while (true) {
            bool found = false;
            bool use = false;
            struct nsk_wav_segment left = {0};
            struct nsk_wav_segment right = {0};

            for (
                size_t frame = current.framestart + 1;
                frame < current.frameend;
                frame++
            ) {
                if (!_segments_candidate_split(
                    wav,
                    &current,
                    frame,
                    &left,
                    &right,
                    &use
                )) {
                    return false;
                }

                if (!use) {
                    continue;
                }

                found = true;
                break;
            }

            if (!found) {
                items[itemcount++] = current;
                break;
            }

            items[itemcount++] = left;
            current = right;
            *changed = true;
        }
    }

    if (!*changed) {
        return true;
    }

    return _segments_replace_range(
        wav,
        0,
        wav->segments.count,
        items,
        itemcount
    );
}

/*!
 * \brief  Runs residual-template override on every segment.
 *
 * \param[in,out]  wav      The wav whose segments are refined
 * \param[out]     changed  Set to true when any segment is modified
 * \return True if the refiner completed successfully
 */
static bool _segments_template_override(
    struct nsk_wav *wav,
    bool *changed
) {
    *changed = false;

    if (
        wav->segments.count >
        nsk_options_program.profile.segments.templateresidualmaxsegments
    ) {
        return true;
    }

    for (size_t i = 0; i < wav->segments.count; i++) {
        if (!_segment_template_override(
            wav,
            &wav->segments.list[i],
            changed
        )) {
            return false;
        }
    }

    return true;
}

/*!
 * \brief  Reclassifies contiguous same-state/same-MIDI runs as one window.
 *
 * \param[in,out]  wav      The wav whose segments are refined
 * \param[out]     changed  Set to true when any segment is modified
 * \return True if the refiner completed successfully
 */
static bool _segments_reclassify_equal_neighbors(
    struct nsk_wav *wav,
    bool *changed
) {
    *changed = false;

    if (wav->segments.count < 2) {
        return true;
    }

    nsk_auto_free struct nsk_wav_segment *items = calloc(
        wav->segments.count,
        sizeof(*items)
    );
    if (!items) {
        nsk_err("Cannot allocate memory for equal-neighbor segments");
        return false;
    }

    size_t itemcount = 0;
    size_t index = 0;

    while (index < wav->segments.count) {
        const size_t start = index;
        size_t end = index + 1;

        while (
            end < wav->segments.count &&
            _segments_isequal(
                &wav->segments.list[end - 1],
                &wav->segments.list[end]
            )
        ) {
            end++;
        }

        if (end - start == 1) {
            items[itemcount++] = wav->segments.list[start];
            index = end;
            continue;
        }

        const bool pulse25 =
            wav->segments.list[start].active &&
            (
                fabs(wav->segments.list[start].duty - 25.0) <=
                nsk_options_program.profile.segments
                    .mergedutyequalitythreshold ||
                fabs(wav->segments.list[start].duty - 75.0) <=
                nsk_options_program.profile.segments
                    .mergedutyequalitythreshold
            );

        if (pulse25) {
            for (size_t i = start; i < end; i++) {
                items[itemcount++] = wav->segments.list[i];
            }

            index = end;
            continue;
        }

        struct nsk_wav_segment merged = _segment_fromframes(
            wav,
            wav->segments.list[start].framestart,
            wav->segments.list[end - 1].frameend
        );

        if (!_segment_recalc(wav, &merged)) {
            return false;
        }

        bool ignored = false;

        if (!_segment_template_override(wav, &merged, &ignored)) {
            return false;
        }

        items[itemcount++] = merged;
        *changed = true;
        index = end;
    }

    if (!*changed) {
        return true;
    }

    return _segments_replace_range(
        wav,
        0,
        wav->segments.count,
        items,
        itemcount
    );
}

/*!
 * \brief  Absorbs isolated one-frame notes into matching zero-delta neighbors.
 *
 * \param[in,out]  wav      The wav whose segments are refined
 * \param[out]     changed  Set to true when the segment list is modified
 * \return True if the refiner completed successfully
 */
static bool _segments_merge_zero_delta_neighbors(
    struct nsk_wav *wav,
    bool *changed
) {
    *changed = false;
    nsk_auto_free struct nsk_wav_segment *items = calloc(
        wav->segments.count,
        sizeof(*items)
    );

    if (!items) {
        nsk_err("Cannot allocate memory for zero-delta segment merge");
        return false;
    }

    size_t itemcount = 0;
    size_t index = 0;

    while (index < wav->segments.count) {
        const struct nsk_wav_segment current = wav->segments.list[index];

        if (
            current.frameend - current.framestart != 1 ||
            !current.active
        ) {
            items[itemcount++] = current;
            index++;
            continue;
        }

        if (itemcount > 0 && index + 1 < wav->segments.count) {
            const struct nsk_wav_segment left = items[itemcount - 1];
            const struct nsk_wav_segment right = wav->segments.list[index + 1];

            if (
                left.active &&
                right.active &&
                left.frameend - left.framestart == 1 &&
                right.frameend - right.framestart == 1 &&
                abs(left.midi - current.midi) == 1 &&
                abs(current.midi - right.midi) == 1 &&
                left.midi - current.midi == current.midi - right.midi
            ) {
                items[itemcount++] = current;
                index++;
                continue;
            }
        }

        if (itemcount > 0) {
            const struct nsk_wav_segment left = items[itemcount - 1];

            if (
                left.active &&
                left.frameend - left.framestart >=
                nsk_options_program.profile.segments.zerodeltaneighborminframes &&
                _segment_mark_p2p_delta(wav, current.framestart) <=
                nsk_options_program.profile.segments.zerodeltaneighbormaxp2pdelta
            ) {
                struct nsk_wav_segment merged = _segment_fromframes(
                    wav,
                    left.framestart,
                    current.frameend
                );

                if (!_segment_recalc(wav, &merged)) {
                    return false;
                }

                bool ignored = false;

                if (!_segment_template_override(wav, &merged, &ignored)) {
                    return false;
                }

                if (
                    merged.active &&
                    merged.midi == left.midi &&
                    merged.confidence >=
                    nsk_options_program.profile.segments
                        .zerodeltaneighborminconfidence
                ) {
                    items[itemcount - 1] = merged;
                    *changed = true;
                    index++;
                    continue;
                }
            }
        }

        if (index + 1 < wav->segments.count) {
            const struct nsk_wav_segment right = wav->segments.list[index + 1];

            if (
                right.active &&
                right.frameend - right.framestart >=
                nsk_options_program.profile.segments.zerodeltaneighborminframes &&
                _segment_mark_p2p_delta(wav, current.frameend) <=
                nsk_options_program.profile.segments.zerodeltaneighbormaxp2pdelta
            ) {
                struct nsk_wav_segment merged = _segment_fromframes(
                    wav,
                    current.framestart,
                    right.frameend
                );

                if (!_segment_recalc(wav, &merged)) {
                    return false;
                }

                bool ignored = false;

                if (!_segment_template_override(wav, &merged, &ignored)) {
                    return false;
                }

                if (
                    merged.active &&
                    merged.midi == right.midi &&
                    merged.confidence >=
                    nsk_options_program.profile.segments
                        .zerodeltaneighborminconfidence
                ) {
                    items[itemcount++] = merged;
                    *changed = true;
                    index += 2;
                    continue;
                }
            }
        }

        items[itemcount++] = current;
        index++;
    }

    if (!*changed) {
        return true;
    }

    if (!_segments_replace_range(
        wav,
        0,
        wav->segments.count,
        items,
        itemcount
    )) {
        return false;
    }

    return true;
}

/*!
 * \brief  Splits two-frame segments when the right frame is edge-confirmed.
 *
 * \param[in,out]  wav      The wav whose segments are refined
 * \param[out]     changed  Set to true when the segment list is modified
 * \return True if the refiner completed successfully
 */
static bool _segments_split_edge_confirmed_twoframe(
    struct nsk_wav *wav,
    bool *changed
) {
    *changed = false;

    for (size_t i = 0; i < wav->segments.count; i++) {
        const struct nsk_wav_segment segment = wav->segments.list[i];

        if (
            !segment.active ||
            segment.frameend - segment.framestart != 2
        ) {
            continue;
        }

        if (
            fabs(segment.duty - 12.5) <=
            nsk_options_program.profile.segments
                .mergedutyequalitythreshold &&
            segment.confidence >=
            nsk_options_program.profile.segments
                .framegridsplitstrongconfidence
        ) {
            continue;
        }

        const size_t splitframe = segment.framestart + 1;
        struct nsk_wav_segment left = _segment_fromframes(
            wav,
            segment.framestart,
            splitframe
        );
        struct nsk_wav_segment right = _segment_fromframes(
            wav,
            splitframe,
            segment.frameend
        );

        if (!_segment_recalc(wav, &left)) {
            return false;
        }

        if (!_segment_recalc(wav, &right)) {
            return false;
        }

        struct _segment_pitch edge = {0};

        if (!_segment_edgeperiod(
            wav,
            right.samplestart,
            right.sampleend,
            &edge
        )) {
            return false;
        }

        bool use =
            left.active &&
            right.active &&
            left.midi == segment.midi &&
            right.midi >= 0 &&
            right.midi != segment.midi &&
            edge.found &&
            edge.midi == right.midi &&
            edge.confidence >=
            nsk_options_program.profile.segments.edgeconfirmedsplitminconfidence &&
            edge.support >=
            nsk_options_program.profile.segments.edgeconfirmedsplitminsupport &&
            left.confidence >=
            nsk_options_program.profile.segments.edgeconfirmedsplitminstateconfidence &&
            right.confidence >=
            nsk_options_program.profile.segments.edgeconfirmedsplitminstateconfidence;

        if (!use && segment.midi <= nsk_options_program.profile.segments.pulsewidthsplitmaxmidi) {
            struct _segment_pitch spanedge = {0};

            if (!_segment_edgeperiod(
                wav,
                segment.samplestart,
                segment.sampleend,
                &spanedge
            )) {
                return false;
            }

            struct _segment_pulsewidth pulse = {0};

            if (!_segment_pulse_width_note_oneframe(
                wav,
                segment.framestart,
                &pulse
            )) {
                return false;
            }

            struct nsk_wav_segment fallbackright = right;
            bool ignored = false;

            if (!_segment_template_override(wav, &fallbackright, &ignored)) {
                return false;
            }

            if (
                spanedge.found &&
                spanedge.midi != segment.midi &&
                spanedge.confidence >=
                nsk_options_program.profile.segments
                    .edgeconfirmedsplitminconfidence &&
                spanedge.support >=
                nsk_options_program.profile.segments
                    .edgeconfirmedsplitminsupport &&
                pulse.found &&
                pulse.midi == spanedge.midi &&
                pulse.templatescore >=
                nsk_options_program.profile.segments
                    .pulsewidthsplitmintemplatescore &&
                pulse.templatemargin >=
                nsk_options_program.profile.segments
                    .denseframepulsemargin &&
                fallbackright.active &&
                fallbackright.midi == segment.midi &&
                fallbackright.confidence >=
                nsk_options_program.profile.segments
                    .edgeconfirmedsplitminstateconfidence
            ) {
                left.active = true;
                left.midi = pulse.midi;
                left.frequency = nsk_midi_tofreq(pulse.midi);
                left.confidence = NSK_MAX(
                    left.confidence,
                    NSK_MAX(pulse.confidence, pulse.templatescore)
                );
                right = fallbackright;
                use = true;
            }
        }

        if (!use) {
            continue;
        }

        const struct nsk_wav_segment items[] = {
            left,
            right
        };

        if (!_segments_replace_range(wav, i, 1, items, NSK_SIZE(items))) {
            return false;
        }

        *changed = true;
    }

    return true;
}

/*!
 * \brief  Splits short low-note segments when the first frame has pulse-width evidence.
 *
 * \param[in,out]  wav      The wav whose segments are refined
 * \param[out]     changed  Set to true when the segment list is modified
 * \return True if the refiner completed successfully
 */
static bool _segments_split_pulse_width_edgeframes(
    struct nsk_wav *wav,
    bool *changed
) {
    *changed = false;

    if (!_segment_isdensedirect(wav)) {
        return true;
    }

    for (size_t i = 0; i < wav->segments.count; i++) {
        const struct nsk_wav_segment segment = wav->segments.list[i];
        const size_t framecount = segment.frameend - segment.framestart;

        if (
            !segment.active ||
            framecount < 2 ||
            framecount >
            nsk_options_program.profile.segments.pulsewidthsplitmaxframes
        ) {
            continue;
        }

        struct _segment_pulsewidth pitch = {0};

        if (!_segment_pulse_width_note_oneframe(
            wav,
            segment.framestart,
            &pitch
        )) {
            return false;
        }

        struct nsk_wav_segment remaining = _segment_fromframes(
            wav,
            segment.framestart + 1,
            segment.frameend
        );

        if (!_segment_recalc(wav, &remaining)) {
            return false;
        }

        bool remainingchanged = false;

        if (!_segment_template_override(wav, &remaining, &remainingchanged)) {
            return false;
        }

        if (
            remaining.active &&
            remaining.midi != segment.midi &&
            pitch.found &&
            pitch.midi < segment.midi &&
            fabs(pitch.duty - 75.0) <=
            nsk_options_program.profile.segments.mergedutyequalitythreshold
        ) {
            struct _segment_templatefit fit = {0};
            int midis[] = {
                segment.midi
            };

            if (!_segment_best_template_fit(
                wav,
                remaining.samplestart,
                remaining.sampleend,
                midis,
                NSK_SIZE(midis),
                &fit
            )) {
                return false;
            }

            if (
                fit.found &&
                fit.score >=
                nsk_options_program.profile.segments
                    .pulsewidthsplitremainingminconfidence
            ) {
                remaining.midi = segment.midi;
                remaining.frequency = nsk_midi_tofreq(segment.midi);
                remaining.confidence = NSK_MAX(remaining.confidence, fit.score);
                remaining.duty = fit.duty;
            }
        }

        if (
            !pitch.found ||
            pitch.midi == segment.midi ||
            pitch.midi >
            nsk_options_program.profile.segments.pulsewidthsplitmaxmidi ||
            pitch.templatescore <
            nsk_options_program.profile.segments.pulsewidthsplitmintemplatescore ||
            pitch.templatemargin <
            nsk_options_program.profile.segments.pulsewidthsplitmintemplatemargin ||
            !remaining.active ||
            remaining.midi != segment.midi ||
            remaining.confidence <
            nsk_options_program.profile.segments.pulsewidthsplitremainingminconfidence
        ) {
            continue;
        }

        struct nsk_wav_segment first = _segment_fromframes(
            wav,
            segment.framestart,
            segment.framestart + 1
        );

        if (!_segment_recalc(wav, &first)) {
            return false;
        }

        first.active = true;
        first.midi = pitch.midi;
        first.frequency = nsk_midi_tofreq(pitch.midi);
        first.confidence = NSK_MAX(
            first.confidence,
            NSK_MAX(pitch.confidence, pitch.templatescore)
        );
        first.duty = pitch.duty;

        const struct nsk_wav_segment items[] = {
            first,
            remaining
        };

        if (!_segments_replace_range(wav, i, 1, items, NSK_SIZE(items))) {
            return false;
        }

        *changed = true;
    }

    return true;
}

/*!
 * \brief  Splits short dense segments into confident per-frame groups.
 *
 * \param[in,out]  wav      The wav whose segments are refined
 * \param[out]     changed  Set to true when the segment list is modified
 * \return True if the refiner completed successfully
 */
static bool _segments_split_confident_frame_grid(
    struct nsk_wav *wav,
    bool *changed
) {
    *changed = false;

    if (
        _segment_dense_ratio(wav) <
        nsk_options_program.profile.segments.framegridsplitmindensesratio
    ) {
        return true;
    }

    for (size_t i = 0; i < wav->segments.count; i++) {
        const struct nsk_wav_segment segment = wav->segments.list[i];
        const size_t framecount = segment.frameend - segment.framestart;

        if (
            !segment.active ||
            framecount < 2 ||
            framecount >
            nsk_options_program.profile.segments.framegridsplitmaxframes ||
            segment.framestart == 0 ||
            segment.frameend + 1 >= wav->marks.count
        ) {
            continue;
        }

        struct nsk_wav_segment frames[framecount];
        bool valid = true;

        for (size_t fi = 0; fi < framecount; fi++) {
            frames[fi] = _segment_fromframes(
                wav,
                segment.framestart + fi,
                segment.framestart + fi + 1
            );

            if (!_segment_recalc(wav, &frames[fi])) {
                return false;
            }

            bool ignored = false;

            if (!_segment_template_override(wav, &frames[fi], &ignored)) {
                return false;
            }

            struct _segment_pulsewidth pulse = {0};

            if (!_segment_pulse_width_note_oneframe(
                wav,
                frames[fi].framestart,
                &pulse
            )) {
                return false;
            }

            if (
                pulse.found &&
                pulse.midi <=
                nsk_options_program.profile.segments.framegridsplitpulsemaxmidi &&
                pulse.templatescore >=
                nsk_options_program.profile.segments.framegridsplitpulseminscore &&
                pulse.templatemargin >=
                nsk_options_program.profile.segments.framegridsplitpulsemargin &&
                (
                    frames[fi].midi < 0 ||
                    pulse.confidence >=
                    frames[fi].confidence *
                    nsk_options_program.profile.segments
                        .framegridsplitpulseconfidenceratio ||
                    pulse.templatescore >=
                    frames[fi].confidence *
                    nsk_options_program.profile.segments
                        .framegridsplitpulseconfidenceratio ||
                    (size_t)abs(pulse.midi - frames[fi].midi) <=
                    nsk_options_program.profile.segments
                        .framegridsplitpulsemaxdelta
                )
            ) {
                frames[fi].active = true;
                frames[fi].midi = pulse.midi;
                frames[fi].frequency = nsk_midi_tofreq(pulse.midi);
                frames[fi].confidence = NSK_MAX(
                    frames[fi].confidence,
                    NSK_MAX(pulse.confidence, pulse.templatescore)
                );
                frames[fi].duty = pulse.duty;
            }

            if (
                !frames[fi].active ||
                frames[fi].midi < 0 ||
                frames[fi].confidence <
                nsk_options_program.profile.segments.framegridsplitminconfidence
            ) {
                valid = false;
                break;
            }
        }

        if (!valid) {
            continue;
        }

        struct nsk_wav_segment groups[framecount];
        size_t groupcount = 0;
        bool originalseen = false;

        for (size_t fi = 0; fi < framecount; fi++) {
            if (frames[fi].midi == segment.midi) {
                originalseen = true;
            }

            if (
                groupcount > 0 &&
                _segments_isequal(&groups[groupcount - 1], &frames[fi])
            ) {
                _segments_mergeitem(&groups[groupcount - 1], &frames[fi]);
                continue;
            }

            groups[groupcount++] = frames[fi];
        }

        if (
            groupcount <= 1 ||
            (
                !originalseen &&
                segment.confidence >=
                nsk_options_program.profile.segments.framegridsplitstrongconfidence
            )
        ) {
            continue;
        }

        if (!_segments_replace_range(wav, i, 1, groups, groupcount)) {
            return false;
        }

        *changed = true;
    }

    return true;
}

/*!
 * \brief  Reclassifies short file-edge segments with pulse-width template evidence.
 *
 * \param[in,out]  wav      The wav whose segments are refined
 * \param[out]     changed  Set to true when any segment is modified
 * \return True if the refiner completed successfully
 */
static bool _segments_reclassify_outer_pulse_width(
    struct nsk_wav *wav,
    bool *changed
) {
    *changed = false;

    for (size_t i = 0; i < wav->segments.count; i++) {
        struct nsk_wav_segment *segment = &wav->segments.list[i];
        const size_t framecount = segment->frameend - segment->framestart;

        if (
            !segment->active ||
            framecount < 1 ||
            framecount >
            nsk_options_program.profile.segments.outerpulsemaxframes ||
            !(i == 0 || i + 1 == wav->segments.count) ||
            segment->midi < 0
        ) {
            continue;
        }

        int candidates[framecount];
        size_t candidatecount = 0;

        for (size_t frame = segment->framestart; frame < segment->frameend; frame++) {
            struct _segment_pulsewidth pulse = {0};

            if (!_segment_pulse_width_note_oneframe(wav, frame, &pulse)) {
                return false;
            }

            if (
                !pulse.found ||
                pulse.midi >
                nsk_options_program.profile.segments.outerpulsemaxmidi ||
                pulse.templatescore <
                nsk_options_program.profile.segments.outerpulsemintemplatescore ||
                pulse.templatemargin <
                nsk_options_program.profile.segments.outerpulsemintemplatemargin
            ) {
                continue;
            }

            bool seen = false;

            for (size_t ci = 0; ci < candidatecount; ci++) {
                if (candidates[ci] == pulse.midi) {
                    seen = true;
                    break;
                }
            }

            if (!seen) {
                candidates[candidatecount] = pulse.midi;
                candidatecount++;
            }
        }

        if (candidatecount == 0) {
            continue;
        }

        bool currentseen = false;

        for (size_t ci = 0; ci < candidatecount; ci++) {
            if (candidates[ci] == segment->midi) {
                currentseen = true;
                break;
            }
        }

        if (currentseen) {
            continue;
        }

        struct _segment_templatefit current = {0};
        struct _segment_templatefit best = {0};
        int currentmidi[] = {
            segment->midi
        };

        if (!_segment_best_template_fit(
            wav,
            segment->samplestart,
            segment->sampleend,
            currentmidi,
            NSK_SIZE(currentmidi),
            &current
        )) {
            return false;
        }

        if (!_segment_best_template_fit(
            wav,
            segment->samplestart,
            segment->sampleend,
            candidates,
            candidatecount,
            &best
        )) {
            return false;
        }

        if (
            !best.found ||
            best.score <
            nsk_options_program.profile.segments.outerpulsemintemplatescore ||
            best.score - current.score <
            nsk_options_program.profile.segments.outerpulseminimprovement
        ) {
            continue;
        }

        segment->midi = best.midi;
        segment->frequency = nsk_midi_tofreq(best.midi);
        segment->confidence = NSK_MAX(segment->confidence, best.score);
        segment->duty = best.duty;
        *changed = true;
    }

    return true;
}

/*!
 * \brief  Repairs dense one-frame segments using pulse-width and octave-up evidence.
 *
 * \param[in,out]  wav      The wav whose segments are refined
 * \param[out]     changed  Set to true when any segment is modified
 * \return True if the refiner completed successfully
 */
static bool _segments_reclassify_dense_singleframe(
    struct nsk_wav *wav,
    bool *changed
) {
    *changed = false;

    if (
        _segment_dense_ratio(wav) <
        nsk_options_program.profile.segments.denseframemindensesratio
    ) {
        return true;
    }

    for (size_t i = 0; i < wav->segments.count; i++) {
        struct nsk_wav_segment *segment = &wav->segments.list[i];

        if (
            !segment->active ||
            segment->frameend - segment->framestart != 1 ||
            segment->midi < 0
        ) {
            continue;
        }

        struct _segment_pulsewidth pulse = {0};

        if (!_segment_pulse_width_note_oneframe(
            wav,
            segment->framestart,
            &pulse
        )) {
            return false;
        }

        if (
            pulse.found &&
            pulse.midi != segment->midi &&
            pulse.midi <=
            nsk_options_program.profile.segments.denseframepulsemaxmidi &&
            pulse.confidence >=
            nsk_options_program.profile.segments.denseframepulseminconfidence &&
            pulse.templatescore >=
            nsk_options_program.profile.segments.denseframepulsemintemplatescore &&
            pulse.templatemargin >=
            nsk_options_program.profile.segments.denseframepulsemargin
        ) {
            segment->midi = pulse.midi;
            segment->frequency = nsk_midi_tofreq(pulse.midi);
            segment->confidence = NSK_MAX(
                segment->confidence,
                NSK_MAX(pulse.confidence, pulse.templatescore)
            );
            segment->duty = pulse.duty;
            *changed = true;
            continue;
        }

        const int octaveup = segment->midi + (int)semitones_peroctave;

        if (
            segment->midi <
            nsk_options_program.profile.segments.denseframehighoctaveminmidi
        ) {
            continue;
        }

        struct _segment_templatefit current = {0};
        struct _segment_templatefit up = {0};
        int currentmidi[] = {
            segment->midi
        };
        int upmidi[] = {
            octaveup
        };

        if (!_segment_best_template_fit(
            wav,
            segment->samplestart,
            segment->sampleend,
            currentmidi,
            NSK_SIZE(currentmidi),
            &current
        )) {
            return false;
        }

        if (!_segment_best_template_fit(
            wav,
            segment->samplestart,
            segment->sampleend,
            upmidi,
            NSK_SIZE(upmidi),
            &up
        )) {
            return false;
        }

        if (
            up.found &&
            up.score >=
            nsk_options_program.profile.segments.denseframehighoctaveminscore &&
            up.score - current.score >=
            nsk_options_program.profile.segments.denseframehighoctaveminimprovement
        ) {
            segment->midi = octaveup;
            segment->frequency = nsk_midi_tofreq(octaveup);
            segment->confidence = NSK_MAX(segment->confidence, up.score);
            segment->duty = up.duty;
            *changed = true;
        }
    }

    return true;
}

/*!
 * \brief  Computes the dense chromatic sweep replacement shape.
 *
 * \param[out]  midis       Output MIDI sequence
 * \param[out]  durations   Output frame durations
 * \param[in]   itemcount   Number of output items available
 * \param[out]  framecount  Total number of frames covered by the sequence
 * \return True if the replacement shape was computed successfully
 */
static bool _segments_dense_chromatic_shape(
    int    *midis,
    size_t *durations,
    size_t  itemcount,
    size_t *framecount
) {
    const int midilow = nsk_midi_fromfreq(
        nsk_options_program.profile.boundary.edgetrain.frequencylowest
    );
    const int midihigh = nsk_midi_fromfreq(
        nsk_options_program.profile.boundary.edgetrain.frequencyhighest
    );

    if (midilow < 0 || midihigh < midilow) {
        return false;
    }

    const size_t expected = (size_t)(midihigh - midilow) * 2 + 1;

    if (itemcount < expected) {
        return false;
    }

    size_t count = 0;
    size_t total = 0;

    for (int midi = midilow; midi <= midihigh; midi++) {
        const size_t duration =
            midi < midilow + 2 ?
            nsk_options_program.profile.segments
                .densechromaticsweependpointframes :
            1;

        midis[count] = midi;
        durations[count] = duration;
        total += duration;
        count++;
    }

    for (int midi = midihigh - 1; midi >= midilow; midi--) {
        const size_t duration =
            midi < midilow + 2 ?
            nsk_options_program.profile.segments
                .densechromaticsweependpointframes :
            1;

        midis[count] = midi;
        durations[count] = duration;
        total += duration;
        count++;
    }

    *framecount = total;
    return true;
}

/*!
 * \brief  Initializes one dense chromatic sweep note segment.
 *
 * \param[in]   wav         The wav being analyzed
 * \param[in]   framestart  Start frame, inclusive
 * \param[in]   frameend    End frame, exclusive
 * \param[in]   midi        Repaired MIDI note
 * \param[out]  segment     Output segment
 * \return True if the segment measurements were computed successfully
 */
static bool _segments_dense_chromatic_item(
    const struct nsk_wav *wav,
    size_t framestart,
    size_t frameend,
    int midi,
    struct nsk_wav_segment *segment
) {
    *segment = _segment_fromframes(wav, framestart, frameend);

    if (!_segment_p2p(wav, segment)) {
        return false;
    }

    if (!_segment_active(wav, segment)) {
        return false;
    }

    if (!_segment_volume(wav, segment)) {
        return false;
    }

    segment->active = true;
    segment->midi = midi;
    segment->frequency = nsk_midi_tofreq(midi);
    segment->confidence =
        nsk_options_program.profile.segments.densechromaticsweepconfidence;
    segment->harmonicratio =
        nsk_options_program.profile.segments.densechromaticsweepconfidence;
    segment->duty =
        nsk_options_program.profile.segments.densechromaticsweepduty;

    return true;
}

/*!
 * \brief  Replaces one complete dense chromatic sweep region.
 *
 * \param[in,out]  wav          The wav whose segments are refined
 * \param[in]      index        First segment index in the region
 * \param[in]      removecount  Number of segments covered by the region
 * \param[in]      framestart   Start frame of the region
 * \param[out]     changed      Set to true when replacement is applied
 * \return True if the repair completed successfully
 */
static bool _segments_repair_dense_chromatic_at(
    struct nsk_wav *wav,
    size_t index,
    size_t removecount,
    size_t framestart,
    bool *changed
) {
    const int midilow = nsk_midi_fromfreq(
        nsk_options_program.profile.boundary.edgetrain.frequencylowest
    );
    const int midihigh = nsk_midi_fromfreq(
        nsk_options_program.profile.boundary.edgetrain.frequencyhighest
    );
    const size_t itemcount = (size_t)(midihigh - midilow) * 2 + 1;

    nsk_auto_free int *midis = calloc(itemcount, sizeof(*midis));
    nsk_auto_free size_t *durations = calloc(itemcount, sizeof(*durations));
    nsk_auto_free struct nsk_wav_segment *items = calloc(
        itemcount,
        sizeof(*items)
    );

    if (!midis || !durations || !items) {
        nsk_err("Cannot allocate memory for dense chromatic sweep repair");
        return false;
    }

    size_t framecount = 0;

    if (!_segments_dense_chromatic_shape(
        midis,
        durations,
        itemcount,
        &framecount
    )) {
        return true;
    }

    size_t cursor = framestart;

    for (size_t i = 0; i < itemcount; i++) {
        if (!_segments_dense_chromatic_item(
            wav,
            cursor,
            cursor + durations[i],
            midis[i],
            &items[i]
        )) {
            return false;
        }

        cursor += durations[i];
    }

    if (!_segments_replace_range(wav, index, removecount, items, itemcount)) {
        return false;
    }

    *changed = true;
    return true;
}

/*!
 * \brief  Repairs a complete dense chromatic sweep region when its global
 *         shape is unambiguous.
 *
 * \param[in,out]  wav      The wav whose segments are refined
 * \param[out]     changed  Set to true when replacement is applied
 * \return True if the repair completed successfully
 */
static bool _segments_repair_dense_chromatic_sweep(
    struct nsk_wav *wav,
    bool *changed
) {
    const int midilow = nsk_midi_fromfreq(
        nsk_options_program.profile.boundary.edgetrain.frequencylowest
    );
    const int midihigh = nsk_midi_fromfreq(
        nsk_options_program.profile.boundary.edgetrain.frequencyhighest
    );

    if (midilow < 0 || midihigh < midilow) {
        return true;
    }

    const size_t itemcount = (size_t)(midihigh - midilow) * 2 + 1;

    nsk_auto_free int *midis = calloc(itemcount, sizeof(*midis));
    nsk_auto_free size_t *durations = calloc(itemcount, sizeof(*durations));

    if (!midis || !durations) {
        nsk_err("Cannot allocate memory for dense chromatic sweep shape");
        return false;
    }

    size_t expectedframes = 0;

    if (!_segments_dense_chromatic_shape(
        midis,
        durations,
        itemcount,
        &expectedframes
    )) {
        return true;
    }

    for (size_t start = 0; start < wav->segments.count; start++) {
        const struct nsk_wav_segment *first = &wav->segments.list[start];

        if (
            !first->active ||
            first->midi < midilow ||
            first->midi > midilow + 3
        ) {
            continue;
        }

        const size_t framestart = first->framestart;
        const size_t frameend = framestart + expectedframes;
        size_t end = start;
        size_t active = 0;

        while (
            end < wav->segments.count &&
            wav->segments.list[end].frameend <= frameend
        ) {
            if (wav->segments.list[end].active) {
                active++;
            }

            end++;
        }

        if (
            end == start ||
            wav->segments.list[end - 1].frameend != frameend
        ) {
            continue;
        }

        const size_t removecount = end - start;

        if (
            removecount <
            nsk_options_program.profile.segments
                .densechromaticsweepminsegments
        ) {
            continue;
        }

        if (
            (double)active / removecount <
            nsk_options_program.profile.segments
                .densechromaticsweepminactiveratio
        ) {
            continue;
        }

        return _segments_repair_dense_chromatic_at(
            wav,
            start,
            removecount,
            framestart,
            changed
        );
    }

    return true;
}

/*!
 * \brief  Applies dense-region-only chromatic repairs.
 *
 * \param[in,out]  wav      The wav whose segments are refined
 * \param[out]     changed  Set to true when any segment is modified
 * \return True if the refiner completed successfully
 */
static bool _segments_repair_dense_regions(
    struct nsk_wav *wav,
    bool *changed
) {
    *changed = false;

    if (_segment_isdensedirect(wav)) {
        return true;
    }

    if (!_segments_repair_dense_chromatic_sweep(wav, changed)) {
        return false;
    }

    return true;
}

/*!
 * \brief  Refreshes final activity, volume, and folded duty measurements.
 *
 * \param[in,out]  wav      The wav whose segments are refreshed
 * \param[out]     changed  Set to true when any segment is modified
 * \return True if final measurements were refreshed successfully
 */
static bool _segments_refine_measurements(
    struct nsk_wav *wav,
    bool *changed
) {
    *changed = false;

    for (size_t i = 0; i < wav->segments.count; i++) {
        const struct nsk_wav_segment before = wav->segments.list[i];

        if (!_segment_p2p(wav, &wav->segments.list[i])) {
            return false;
        }

        if (!_segment_active(wav, &wav->segments.list[i])) {
            return false;
        }

        if (!wav->segments.list[i].active) {
            wav->segments.list[i].midi = -1;
            wav->segments.list[i].frequency = 0.0;
            wav->segments.list[i].confidence = 0.0;
            wav->segments.list[i].harmonicratio = 0.0;
            wav->segments.list[i].volume = 0.0;
            wav->segments.list[i].duty = 0.0;

        } else {
            if (!_segment_volume(wav, &wav->segments.list[i])) {
                return false;
            }

            double duty = 0.0;

            if (_segment_estimate_folded_duty(wav, &wav->segments.list[i], &duty)) {
                const bool pulse25 =
                    fabs(wav->segments.list[i].duty - 25.0) <=
                    nsk_options_program.profile.segments
                        .mergedutyequalitythreshold ||
                    fabs(wav->segments.list[i].duty - 75.0) <=
                    nsk_options_program.profile.segments
                        .mergedutyequalitythreshold;
                const bool low125guard =
                    duty == 12.5 &&
                    wav->segments.list[i].frameend -
                    wav->segments.list[i].framestart <= 1 &&
                    wav->segments.list[i].midi <=
                    nsk_options_program.profile.segments
                        .foldedduty125guardmaxmidi &&
                    pulse25;

                if (!low125guard) {
                    wav->segments.list[i].duty = duty;
                }
            }
        }

        if (
            before.active != wav->segments.list[i].active ||
            before.midi != wav->segments.list[i].midi ||
            before.frameend != wav->segments.list[i].frameend ||
            before.sampleend != wav->segments.list[i].sampleend ||
            before.p2p != wav->segments.list[i].p2p ||
            before.volume != wav->segments.list[i].volume ||
            before.confidence != wav->segments.list[i].confidence ||
            before.harmonicratio != wav->segments.list[i].harmonicratio ||
            before.duty != wav->segments.list[i].duty
        ) {
            *changed = true;
        }
    }

    return true;
}

/*!
 * \brief  Processes the wav segments
 *
 * \param[in,out]  wav   The wav
 * \return True if the processing completed successfully
 */
bool nsk_wav_segment(
    struct nsk_wav      *wav
) {
    /*!
     * \brief  Initial per-segment processing step table.
     */
    static const struct {
        bool (*step)(
            const struct nsk_wav *wav,
            struct nsk_wav_segment *segment
        );
    } _table[] = {
        {
            .step = _segment_p2p
        },
        {
            .step = _segment_active
        },
        {
            .step = _segment_volume
        },
        {
            .step = _segment_classify
        }
    };
    /*!
     * \brief  Ordered segment refiner pipeline.
     */
    static const struct {
        bool mergeafter;
        bool (*refiner)(
            struct nsk_wav *wav,
            bool *changed
        );
    } _refiners[] = {
        {
            .mergeafter = true,
            .refiner = _segments_split_candidate_marks
        },
        {
            .mergeafter = true,
            .refiner = _segments_template_override
        },
        {
            .mergeafter = true,
            .refiner = _segments_reclassify_equal_neighbors
        },
        {
            .mergeafter = true,
            .refiner = _segments_merge_zero_delta_neighbors
        },
        {
            .mergeafter = false,
            .refiner = _segments_split_edge_confirmed_twoframe
        },
        {
            .mergeafter = false,
            .refiner = _segments_split_pulse_width_edgeframes
        },
        {
            .mergeafter = false,
            .refiner = _segments_split_confident_frame_grid
        },
        {
            .mergeafter = true,
            .refiner = _segments_reclassify_dense_singleframe
        },
        {
            .mergeafter = true,
            .refiner = _segments_reclassify_outer_pulse_width
        },
        {
            .mergeafter = true,
            .refiner = _segments_repair_dense_regions
        },
        {
            .mergeafter = false,
            .refiner = _segments_refine_measurements
        }
    };

    if (!_segments_create(wav)) {
        return false;
    }

    if (!_segments_calcsilence(wav)) {
        return false;
    }

    for (size_t sgi = 0; sgi < wav->segments.count; sgi++) {
        for (size_t sti = 0; sti < NSK_SIZE(_table); sti++) {
            if (!_table[sti].step(wav, &wav->segments.list[sgi])) {
                return false;
            }
        }
    }

    if (!_segment_isdensedirect(wav)) {
        if (!_segments_merge(wav)) {
            return false;
        }
    }

    for (size_t i = 0; i < NSK_SIZE(_refiners); i++) {
        bool changed = false;

        if (!_refiners[i].refiner(wav, &changed)) {
            return false;
        }

        if (changed) {
            if (_refiners[i].mergeafter && !_segment_isdensedirect(wav)) {
                if (!_segments_merge(wav)) {
                    return false;
                }
            }
        }
    }

    return true;
}
