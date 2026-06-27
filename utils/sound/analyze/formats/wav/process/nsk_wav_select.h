#pragma once

#include <stdbool.h>

#include "../nsk_wav_type.h"

/*!
 * \brief  Accepts the time marks that pass the selection
 *
 * \param[in,out]  wav   The wav
 * \return True if selection completed successfully
 */
bool nsk_wav_select(
    struct nsk_wav      *wav
);