#pragma once

#include <stdbool.h>

#include "../nsk_wav_type.h"

/*!
 * \brief  Creates mark pools based on the candidates
 *
 * Maps continuous-time boundary candidates onto the configured frame grid.
 * Candidate timestamps are rounded to the nearest frame and accepted only when
 * the grid error is within the quantization window.
 *
 * \param[in,out]  wav   The wav
 * \return True if quantization completed successfully
 */
bool nsk_wav_quantization(
    struct nsk_wav      *wav
);
