#pragma once

#include <stdbool.h>

#include "../../nsk_wav_type.h"

/*!
 * \brief  Finds grid-locked energy-rise boundary candidates
 *
 * For every internal target frame, the detector compares RMS energy in one
 * frame-sized window before the grid time with RMS energy in one frame-sized
 * window after it.  A sufficiently strong positive rise becomes a weak onset
 * candidate placed exactly on the frame grid.
 *
 * \param[in,out]  wav   The wav
 * \return True if processed successfully
 */
bool nsk_wav_bf_grid(
    struct nsk_wav *wav
);
