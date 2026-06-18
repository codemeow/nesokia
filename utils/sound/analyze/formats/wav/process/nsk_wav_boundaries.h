#pragma once

#include <stdbool.h>

#include "../nsk_wav_type.h"

/*!
 * \brief  Finds the boundaries between notes
 *
 * <TODO>
 *
 * \param[in,out]  wav   The wav
 */
bool nsk_wav_boundaries(
    struct nsk_wav      *wav
);