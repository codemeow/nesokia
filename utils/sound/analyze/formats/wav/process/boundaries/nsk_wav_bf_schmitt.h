#pragma once

#include <stdbool.h>

#include "../../nsk_wav_type.h"

/*!
 * \brief  Finds the boundaries by the Schmitt trigger
 *
 * Runs Schmitt-trigger edge preparation over all active spans collected by the
 * RMS envelope detector.  The configured Schmitt window is converted from
 * seconds to samples and clamped to a small minimum size so rolling extrema have
 * enough context to describe a local waveform neighborhood.
 *
 * \param[in,out]  wav   The wav
 * \return True if all spans were processed successfully
 */
bool nsk_wav_bf_schmitt(
    struct nsk_wav *wav
);