#pragma once

#include <stdbool.h>

#include "../nsk_wav_type.h"
#include "types/nsk_wav_ctx_meta.h"

/*!
 * \brief  Decodes classified waveform evidence into final note/rest events
 *
 * \param[in]      wav   Source WAV data
 * \param[in,out]  ctx   Processing context whose final events are decoded
 * \return True if decoding completed successfully
 */
bool nsk_wav_decoder(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
);
