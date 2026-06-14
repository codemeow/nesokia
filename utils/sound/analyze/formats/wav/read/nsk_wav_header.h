#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "../nsk_wav_type.h"

/*!
 * \brief  Reads WAV file header
 *
 * \param[in,out]  file  The file descriptor
 * \param[in,out]  wav   The wav data
 * \return  True if the header is successfully read
 */
bool nsk_wav_read_header(
    FILE           *file,
    struct nsk_wav *wav
);