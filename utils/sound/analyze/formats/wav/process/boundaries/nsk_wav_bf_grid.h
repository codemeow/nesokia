#pragma once

#include <stdbool.h>

#include "../../nsk_wav_type.h"

/*!
 * \brief  Finds grid-locked energy-rise boundary candidates
 *
 * Compares RMS energy around target grid frames and emits onset candidates
 * when energy rises sharply across a frame boundary.
 *
 * \param[in,out]  wav   The wav
 * \return True if processed successfully
 */
bool nsk_wav_bf_grid(
    struct nsk_wav *wav
);
