#pragma once

#include <stdbool.h>

#include "../nsk_wav_type.h"

/*!
 * \brief  Finds the boundaries between notes
 *
 * <TODO>
 *
 * \param[in,out]  wav   The wav
 * \param[in]      cnds  The list of candidates
 */
bool nsk_wav_boundaries(
    struct nsk_wav      *wav,
    struct nsk_wav_cnds *cnds
);