#include "nsk_wav_sg_spectral.h"
#include "nsk_wav_sg_edgeperiod.h"
#include "nsk_wav_sg_state.h"

/*!
 * \brief  Calculates real FFT magnitudes for a windowed signal.
 *
 * \param[in]  input  Input sample array.
 * \param[in]  inputcount  Number of input samples.
 * \param[in]  fftsize  FFT size.
 * \param[out]  magnitude  Output magnitude array.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_rfftmagnitude(
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
 * \brief  Interpolates spectral amplitude at a fractional FFT bin.
 *
 * \param[out]  magnitude  Output magnitude array.
 * \param[in]  bincount  Number of magnitude bins.
 * \param[in]  bin  Fractional FFT bin.
 * \return Calculated floating-point value.
 */
double nsk_wav_sg_spectralamplitude(
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
 * \brief  Rounds an integer up to the next power of two.
 *
 * \param[in]  x  Integer value to round up.
 * \return Calculated size value.
 */
size_t nsk_wav_sg_ceil_pow2(size_t x) {
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
 * \brief  Classifies pitch and duty for a sample window.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  coresamplestart  Inclusive core sample start.
 * \param[in]  coresampleend  Exclusive core sample end.
 * \param[in,out]  framecount  Number of frames in the analyzed window or repair group.
 * \param[out]  pitch  Calculated pitch result.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_analyze_window(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
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
        sizeof(ctx->samples.value[0])
    );
    if (!fftinput) {
        nsk_err("Cannot allocate memory for FFT input values");
        return false;
    }

    double sum = 0;

    for (size_t i = coresamplestart; i < coresampleend; i++) {
        sum += ctx->samples.value[i];
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
            (ctx->samples.value[coresamplestart + i] - mean) * window;
    }

    size_t basefftsize = nsk_options_program.profile.segments.fftsize;

    if (
        nsk_wav_sg_isdensedirect(wav, ctx) &&
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
        nsk_wav_sg_ceil_pow2(NSK_MAX(8, samplecount))
    );

    const size_t bincount = fftsize / 2 + 1;
    nsk_auto_free double *magnitude = calloc(bincount, sizeof(*magnitude));
    if (!magnitude) {
        nsk_err(
            "Cannot allocate memory for segment magnitude values "
            "(samples=%zu, fft=%zu, bins=%zu)",
            samplecount,
            fftsize,
            bincount
        );
        return false;
    }

    if (!nsk_wav_sg_rfftmagnitude(
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

            amps[harmonic - 1] = nsk_wav_sg_spectralamplitude(
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

    if (!nsk_wav_sg_edgeperiod(
        wav, ctx,
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

    if (!nsk_wav_sg_edgedecide(
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
