#pragma once

#include <stdio.h>
#include "nsk_wav_type.h"

/*!
 * \brief  Reads the WAV data from the provided file
 *
 * \param[in,out]  file  The file descriptor
 * \return Allocated WAV data
 */
struct nsk_wav *nsk_wav_read(FILE *file);