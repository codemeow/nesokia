#pragma once

#include <stdbool.h>

#include "../nsk_wav_type.h"
#include "types/nsk_wav_ctx_meta.h"

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
);
