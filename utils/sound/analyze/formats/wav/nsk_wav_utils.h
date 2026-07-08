#pragma once

#include <stddef.h>

#include "nsk_wav_type.h"

/*!
 * Automatically free the allocated WAV data on block exit
 */
#define nsk_auto_wav __attribute__((cleanup(_nsk_auto_wav)))

/*!
 * \brief  Autocleaner for allocated WAV data
 *
 * \param[in, out]  wav  The WAV data
 */
void _nsk_auto_wav(struct nsk_wav **wav);

/*!
 * \brief  Logs elapsed time between two millisecond timestamps.
 *
 * \param[in] start    Start timestamp, in milliseconds
 * \param[in] end      End timestamp, in milliseconds
 * \param[in] padding  Number of spaces printed before the log entry
 */
void nsk_time_log(
    double start,
    double end,
    size_t padding
);
