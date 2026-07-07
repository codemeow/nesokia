#pragma once

#include <stdbool.h>

#include "../nsk_wav_type.h"

/*!
 * \brief  Processes the wav segments
 *
 * \param[in,out]  wav   The wav
 * \return True if the processing completed successfully
 */
bool nsk_wav_segment(
    struct nsk_wav      *wav
);