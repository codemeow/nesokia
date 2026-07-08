#pragma once

#include <stdbool.h>

#include "../nsk_wav_type.h"
#include "types/nsk_wav_ctx_meta.h"

/*!
 * \brief  Accepts the time marks that pass the selection
 *
 * \param[in]      wav  Source WAV data
 * \param[in,out]  ctx  Processing context containing quantized marks
 * \return True if selection completed successfully
 */
bool nsk_wav_select(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
);
