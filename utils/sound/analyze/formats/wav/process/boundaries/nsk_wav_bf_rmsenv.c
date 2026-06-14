#include <math.h>
#include <float.h>

#include <nsk_util_meta.h>

#include "nsk_wav_bf_rmsenv.h"
#include "../../../../arguments/nsk_args_options.h"

/*!
 * \brief  Creates an array of squared samples values
 *
 * \param[in] wav    The wav
 * \param[in] window Number of elements that will be used as window
 * \return    Allocated array
 */
static double *_bf_rmsenv_squares(
    const struct nsk_wav *wav,
    size_t window
) {
    const size_t half = window / 2;

    double *squared = calloc(
        wav->samples.raw.count + window,
        sizeof(*squared)
    );
    if (!squared) {
        nsk_err(
            "Cannot allocate memory for the array of sample squares"
        );
        return NULL;
    }

    for (size_t i = 0; i < wav->samples.raw.count; i++) {
        squared[i + half] =
            wav->samples.raw.value[i] *
            wav->samples.raw.value[i];
    }

    return squared;
}

/*!
 * \brief  Adds new onset candidate
 *
 * \param[in]      wav   The wav
 * \param[in]      index The index of the sample
 * \param[in,out]  cnds  The list of candidates
 * \return  True if added
 */
static bool _bf_rmsenv_addonset(
    const struct nsk_wav *wav,
    size_t                index,
    struct nsk_wav_cnds  *cnds
) {
    double timestamp = (double)index / wav->format.samplerate;

    return nsk_wav_cnds_new(
        cnds,
        (struct nsk_wav_cnd) {
            .method     = NSK_WAV_CND_METHOD_ENERGY_ONSET,
            .kind       = NSK_WAV_CND_KIND_ENERGY_ONSET,
            .timestamp  = timestamp,
            .strength   = nsk_options_program.profile.boundary.rmsenv.strengthonset,
            .confidence = nsk_options_program.profile.boundary.rmsenv.confidenceonset
        }
    );
}

/*!
 * \brief  Adds new offset candidate
 *
 * \param[in]      wav   The wav
 * \param[in]      index The index of the sample
 * \param[in,out]  cnds  The list of candidates
 * \return  True if added
 */
static bool _bf_rmsenv_addoffset(
    const struct nsk_wav *wav,
    size_t                index,
    struct nsk_wav_cnds  *cnds
) {
    double timestamp = (double)index / wav->format.samplerate;

    return nsk_wav_cnds_new(
        cnds,
        (struct nsk_wav_cnd) {
            .method     = NSK_WAV_CND_METHOD_ENERGY_OFFSET,
            .kind       = NSK_WAV_CND_KIND_ENERGY_OFFSET,
            .timestamp  = timestamp,
            .strength   = nsk_options_program.profile.boundary.rmsenv.strengthoffset,
            .confidence = nsk_options_program.profile.boundary.rmsenv.confidenceoffset
        }
    );
}

/*!
 * \brief  Calculates the RMS envelopes
 *
 * The RMS envelope is a smoothed energy curve with one value per input
 * sample.  Samples are squared first, then a centered rolling window averages
 * those squared values, and the square root converts the averaged power back
 * to an RMS amplitude.
 *
 * The squared input is zero-padded before this function rolls the window over
 * it.  That makes the edge behavior match a centered "same" convolution:
 * missing samples before the beginning or after the end of the WAV contribute
 * zero, but the divisor remains the full window size.
 *
 * The resulting envelope is normalized by its own peak so downstream
 * thresholds can be expressed in the stable 0..1 range, independent of WAV
 * amplitude.
 *
 * \param[in] wav     The wav
 * \param[in] window  The window size
 * \return Allocated normalized RMS envelope, or NULL on allocation failure
 */
static double *_bf_rmsenv_envelopes(
    const struct nsk_wav *wav,
    size_t                window
) {
    nsk_auto_free double *envelope = calloc(
        wav->samples.raw.count,
        sizeof(*envelope)
    );
    if (!envelope) {
        return NULL;
    }

    nsk_auto_free double *squares = _bf_rmsenv_squares(
        wav,
        window
    );
    if (!squares) {
        return NULL;
    }

    double sum = 0.0;
    for (size_t i = 0; i < window; i++) {
        sum += squares[i];
    }

    double max = 0.0;
    for (size_t i = 0; i < wav->samples.raw.count; i++) {
        envelope[i] = sqrt(sum / window);
        if (envelope[i] > max) {
            max = envelope[i];
        }

        sum -= squares[i];
        sum += squares[i + window];
    }

    if (max <= 0.0) {
        /* No energy change detected */
        return nsk_steal(envelope);
    }

    /* Normalize to 0..1 */
    for (size_t i = 0; i < wav->samples.raw.count; i++) {
        envelope[i] /= max;
    }

    return nsk_steal(envelope);
}

/*!
 * \brief  Detects the points of the energy change
 *
 * Converts the normalized RMS envelope into active audio spans using
 * hysteresis.  A span starts only when the envelope reaches the high threshold;
 * once active, it remains active until the envelope falls to the lower
 * threshold.  This prevents small envelope fluctuations around one threshold
 * from splitting a real active region into many short spans.
 *
 * Very short spans are ignored.  Accepted spans produce two boundary
 * candidates: an energy-onset candidate at the span start and an energy-offset
 * candidate at the span end.  The span end is treated as an exclusive boundary
 * position, so a span that reaches the end of the file ends at sample count,
 * not at the last sample index.
 *
 * \param[in]         wav       The wav
 * \param[in]         envelope  RMS envelopes list
 * \param[in,out]     cnds      Candidates
 * \return True if candidates were added successfully
 */
static bool _bf_rmsenv_spansearch(
    const struct nsk_wav *wav,
    const double         *envelope,
    struct nsk_wav_cnds  *cnds
) {
    bool   active    = false;
    size_t spanstart = 0;

    const size_t minsamples = round(
        wav->format.samplerate *
            nsk_options_program.profile.boundary.rmsenv.minactive
    );
    const double treshhi = nsk_options_program.profile.boundary.rmsenv.energytreshold;
    const double treshlo = treshhi * 0.5;

    for (size_t i = 0; i < wav->samples.raw.count; i++) {
        if (!active && envelope[i] >= treshhi) {
            active    = true;
            spanstart = i;
            continue;
        }

        if (active && envelope[i] <= treshlo) {
            const size_t spanend = i;

            if (spanend - spanstart >= minsamples) {
                if (!_bf_rmsenv_addonset (wav, spanstart, cnds) ||
                    !_bf_rmsenv_addoffset(wav, spanend,   cnds)
                ) {
                    return false;
                }
            }

            active = false;
        }
    }

    if (active) {
        const size_t spanend = wav->samples.raw.count;
        if (spanend - spanstart >= minsamples) {
            if (!_bf_rmsenv_addonset (wav, spanstart, cnds) ||
                !_bf_rmsenv_addoffset(wav, spanend,   cnds)
            ) {
                return false;
            }
        }
    }

    return true;
}

/*!
 * \brief  Finds the boundaries by the RMS envelope
 *
 * Implements the energy-boundary part of the detector.  The configured window
 * length is converted from seconds to samples, then the normalized RMS
 * envelope is calculated and scanned for active spans.  This stage does not
 * decide final note boundaries; it only adds energy-onset and energy-offset
 * candidates to the shared candidate list for later grid selection and
 * decoding.
 *
 * \param[in]      wav   The wav
 * \param[in,out]  cnds  The list of candidates
 * \return True if the RMS envelope detector completed successfully
 */
bool nsk_wav_bf_rmsenv(
    const struct nsk_wav *wav,
    struct nsk_wav_cnds  *cnds
) {
    const size_t window = NSK_MAX(
        round(
            wav->format.samplerate *
            nsk_options_program.profile.boundary.rmsenv.energywindow
        ),
        1
    );

    nsk_auto_free double *envelope = _bf_rmsenv_envelopes(
        wav,
        window
    );
    if (!envelope) {
        return false;
    }

    return _bf_rmsenv_spansearch(
        wav,
        envelope,
        cnds
    );
}
