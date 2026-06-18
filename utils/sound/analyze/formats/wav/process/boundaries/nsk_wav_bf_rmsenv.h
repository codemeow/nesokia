#pragma once

#include <stdbool.h>

#include "../../nsk_wav_type.h"

/*!
 * \brief  Finds the boundaries by the RMS envelope
 *
 * <TODO>
 *
 * \param[in,out]  wav   The wav
 */
bool nsk_wav_bf_rmsenv(
    struct nsk_wav *wav
);