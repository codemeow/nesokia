#pragma once

#include <stdbool.h>

#include "../../nsk_wav_type.h"

/*!
 * \brief  Finds the boundaries by the RMS envelope
 *
 * <TODO>
 *
 * \param[in]  wav   The wav
 * \param[in,out]      cnds  The list of candidates
 */
bool nsk_wav_bf_rmsenv(
    const struct nsk_wav *wav,
    struct nsk_wav_cnds  *cnds
);