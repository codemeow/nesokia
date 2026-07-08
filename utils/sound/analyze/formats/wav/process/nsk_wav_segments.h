#pragma once

#include <stdbool.h>

#include "../nsk_wav_type.h"
#include "types/nsk_wav_ctx_meta.h"

/*!
 * \brief  Processes the wav segments
 *
 * \param[in]      wav   Source WAV data
 * \param[in,out]  ctx   Processing context whose segments are processed
 * \return True if the processing completed successfully
 */
bool nsk_wav_segment(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
);
