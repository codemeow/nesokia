#pragma once

#include <stdbool.h>

/*!
 * \brief  Converts the WAV file to music sheets
 *
 * \param[in] filename  WAV file filename
 * \return    True if converted successfully
 */
bool nsk_wav_process(const char *filename);