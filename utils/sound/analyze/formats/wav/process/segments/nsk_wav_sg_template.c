#include "nsk_wav_sg_template.h"
#include "nsk_wav_sg_classify.h"
#include "nsk_wav_sg_edgeperiod.h"
#include "nsk_wav_sg_period.h"

/*!
 * \brief  Scores a pulse template fit for a MIDI note, duty, and phase search.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  samplestart  Inclusive sample start.
 * \param[in]  sampleend  Exclusive sample end.
 * \param[in]  midi  MIDI note value.
 * \param[in]  duty  Pulse duty candidate.
 * \param[out]  fit  Calculated template fit.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_template_fit(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
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
        mean += ctx->samples.value[samplestart + i];
    }

    mean /= count;

    double sst = 0.0;

    for (size_t i = 0; i < count; i++) {
        const double centered = ctx->samples.value[samplestart + i] - mean;
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
                ctx->samples.value[samplestart + i] -
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
                ctx->samples.value[samplestart + i] -
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
 * \brief  Finds the best pulse template fit across MIDI and duty candidates.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  samplestart  Inclusive sample start.
 * \param[in]  sampleend  Exclusive sample end.
 * \param[in,out]  midis  MIDI candidate array.
 * \param[in,out]  midicount  Number of MIDI candidates.
 * \param[out]  best  Best calculated template fit.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_best_template_fit(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
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

            if (!nsk_wav_sg_template_fit(
                wav, ctx,
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
 * \brief  Adds a MIDI candidate to a candidate list if it is not already present.
 *
 * \param[in,out]  midis  MIDI candidate array.
 * \param[in,out]  count  Number of used items.
 * \param[in]  capacity  Maximum number of items available in the destination array.
 * \param[in]  midi  MIDI note value.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_add_candidate_midi(
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
 * \brief  Builds the MIDI candidate set for template fitting.
 *
 * \param[in]  primary  Primary MIDI candidate.
 * \param[in]  edge  Edge-period MIDI candidate or pitch result.
 * \param[in]  period  Period-scoring MIDI candidate.
 * \param[in,out]  framecount  Number of frames in the analyzed window or repair group.
 * \param[in,out]  midis  MIDI candidate array.
 * \param[in,out]  midicount  Number of MIDI candidates.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_template_candidates(
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
            if (!nsk_wav_sg_add_candidate_midi(
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

            if (!nsk_wav_sg_add_candidate_midi(
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
 * \brief  Applies template-fit override logic to a single segment.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in,out]  segment  Segment being analyzed or updated.
 * \param[out]  changed  Set to true when the function modifies segment data.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_segment_template_override(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
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

    nsk_wav_sg_corebounds(wav, ctx, segment, &samplestart, &sampleend);

    struct _segment_pitch edge = {0};
    struct _segment_pitch period = {0};

    if (!nsk_wav_sg_edgeperiod(wav, ctx, samplestart, sampleend, &edge)) {
        return false;
    }

    if (!nsk_wav_sg_period_note(wav, ctx, samplestart, sampleend, &period)) {
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

    if (!nsk_wav_sg_template_candidates(
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

    if (!nsk_wav_sg_best_template_fit(
        wav, ctx,
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

        if (!nsk_wav_sg_best_template_fit(
            wav, ctx,
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
