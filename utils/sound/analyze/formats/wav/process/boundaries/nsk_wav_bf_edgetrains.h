#pragma once

#include <stdbool.h>

#include "../../nsk_wav_type.h"

/*!
 * \brief  Finds the boundaries by edge trains
 *
 * <TODO>
 *
 * \param[in,out]  wav   The wav
 * \return True if all edges were processed successfully
 */
bool nsk_wav_bf_edgetrains(
    struct nsk_wav *wav
);