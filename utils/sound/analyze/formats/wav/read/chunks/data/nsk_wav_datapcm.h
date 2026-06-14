#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>

#include "../../../nsk_wav_type.h"

/*!
 * \brief  Reads one WAV chunk of type DATA in PCM format
 *
 * \param[in,out] file          The file descriptor
 * \param[in,out] wav           The wav data
 * \param[in]     chunksize     The expected size of the chunk
 * \param[in,out] rest          The rest number of chunks bytes to read
 */
bool nsk_wav_reader_datapcm(
    FILE           *file,
    struct nsk_wav *wav,
    ssize_t         chunksize,
    ssize_t        *rest
);