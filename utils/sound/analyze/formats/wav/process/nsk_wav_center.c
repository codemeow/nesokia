#include <math.h>

#include <nsk_util_meta.h>

#include "nsk_wav_center.h"

/*!
 * \brief  Centers and peak-normalizes sample values
 *
 * Center the waveform around the midpoint of its observed amplitude range, then
 * scale it by the maximum absolute value. This removes DC offset and normalizes
 * peak amplitude to 1.0 while preserving pulse/square shape; using midrange
 * instead of arithmetic mean avoids duty-cycle bias for asymmetric pulse waves.
 *
 * \param[in,out]  wav  The wav
 * \param[in]      cnds  The list of candidates (unused)
 */
bool nsk_wav_center(
    struct nsk_wav      *wav,
    struct nsk_wav_cnds *cnds __attribute__((unused))
) {
    wav->samples.raw.center = (
        wav->samples.raw.max +
        wav->samples.raw.min
    ) / 2.0;

    double peak = 0.0;

    for (size_t i = 0; i < wav->samples.raw.count; i++) {
        wav->samples.raw.value[i] -= wav->samples.raw.center;

        double abs_value = fabs(wav->samples.raw.value[i]);
        if (abs_value > peak) {
            peak = abs_value;
        }
    }

    if (peak <= 0.0) {
        nsk_err("WAV peak value cannot be less than 0\n");
        return false;
    }

    for (size_t i = 0; i < wav->samples.raw.count; i++) {
        wav->samples.raw.value[i] /= peak;
    }

    return true;
}