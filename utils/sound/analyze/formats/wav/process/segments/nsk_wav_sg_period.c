#include "nsk_wav_sg_period.h"

/*!
 * \brief  Score delta required to replace the current period candidate.
 */
static const double periodscoretieepsilon = 1e-12;

/*!
 * \brief  Estimates pitch from autocorrelation period scoring.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  samplestart  Inclusive sample start.
 * \param[in]  sampleend  Exclusive sample end.
 * \param[out]  pitch  Calculated pitch result.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_period_note(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
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
        mean += ctx->samples.value[samplestart + i];
    }

    mean /= count;

    double energy = 0.0;

    for (size_t i = 0; i < count; i++) {
        work[i] = ctx->samples.value[samplestart + i] - mean;
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

            if (score > bestscore + periodscoretieepsilon) {
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
