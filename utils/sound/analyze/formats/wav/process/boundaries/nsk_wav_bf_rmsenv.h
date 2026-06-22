#pragma once

#include <stdbool.h>

#include "../../nsk_wav_type.h"

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
 * \param[in,out]      wav   The wav
 * \return True if the RMS envelope detector completed successfully
 */
bool nsk_wav_bf_rmsenv(
    struct nsk_wav *wav
);