#include <math.h>
#include <stdlib.h>

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
 * \param[in]      wav  Source WAV data
 * \param[in,out]  ctx  Processing context receiving centered samples
 * \return True if the working sample buffer was prepared successfully
 */
bool nsk_wav_center(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
) {
    ctx->samples.count = wav->samples.raw.count;
    ctx->samples.max   = wav->samples.raw.max;
    ctx->samples.min   = wav->samples.raw.min;
    ctx->samples.center = (
        wav->samples.raw.max +
        wav->samples.raw.min
    ) / 2.0;
    ctx->samples.value = calloc(
        ctx->samples.count,
        sizeof(*ctx->samples.value)
    );
    if (!ctx->samples.value) {
        nsk_err("Cannot allocate memory for centered samples\n");
        return false;
    }

    double peak = 0.0;

    for (size_t i = 0; i < ctx->samples.count; i++) {
        ctx->samples.value[i] =
            wav->samples.raw.value[i] -
            ctx->samples.center;

        double abs_value = fabs(ctx->samples.value[i]);
        peak = NSK_MAX(peak, abs_value);
    }

    if (peak <= 0.0) {
        nsk_err("WAV peak value cannot be less than 0\n");
        return false;
    }

    for (size_t i = 0; i < ctx->samples.count; i++) {
        ctx->samples.value[i] /= peak;
    }

    return true;
}
