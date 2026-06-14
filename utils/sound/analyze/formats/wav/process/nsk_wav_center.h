#pragma once

#include <stdbool.h>

#include "../nsk_wav_type.h"

/*!
 * \brief  Centers and peak-normalizes sample values
 *
 * Center the waveform around the midpoint of its observed amplitude range, then
 * scale it by the maximum absolute value. This removes DC offset and normalizes
 * peak amplitude to 1.0 while preserving pulse/square shape; using midrange
 * instead of arithmetic mean avoids duty-cycle bias for asymmetric pulse waves.
 *
 * \param[in,out]  wav  The wav
 */
bool nsk_wav_center(struct nsk_wav *wav);