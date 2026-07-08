#pragma once

#include <stdbool.h>

#include "../../nsk_wav_type.h"
#include "../types/nsk_wav_ctx_meta.h"

/*!
 * \brief  Finds grid-locked energy-rise boundary candidates
 *
 * For every internal target frame, the detector compares RMS energy in one
 * frame-sized window before the grid time with RMS energy in one frame-sized
 * window after it.  A sufficiently strong positive rise becomes a weak onset
 * candidate placed exactly on the frame grid.
 *
 * \param[in]      wav  Source WAV data
 * \param[in,out]  ctx  Processing context receiving candidates
 * \return True if processed successfully
 */
bool nsk_wav_bf_grid(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
);
