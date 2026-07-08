#pragma once

#include <stdbool.h>

#include "../../nsk_wav_type.h"
#include "../types/nsk_wav_ctx_meta.h"

/*!
 * \brief  Finds the boundaries by the Schmitt trigger
 *
 * Runs Schmitt-trigger edge preparation over all active spans collected by the
 * RMS envelope detector.  The configured Schmitt window is converted from
 * seconds to samples and clamped to a small minimum size so rolling extrema have
 * enough context to describe a local waveform neighborhood.
 *
 * \param[in]      wav   Source WAV data
 * \param[in,out]  ctx   Processing context containing spans and receiving candidates
 * \return True if all spans were processed successfully
 */
bool nsk_wav_bf_schmitt(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
);
