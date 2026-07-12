#pragma once

#include <stdbool.h>

#include "../types/nsk_wav_ctx_meta.h"
#include "../../nsk_wav_type.h"

/*!
 * \brief  Converts the chosen segment hypothesis into final note/rest events.
 *
 * \param[in]      wav  Source WAV data.
 * \param[in,out]  ctx  Processing context receiving decoded events.
 * \return True if decoded events were created successfully.
 */
bool nsk_wav_dc_notes_from_segments(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx *ctx
);
