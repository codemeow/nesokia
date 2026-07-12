#pragma once

#include <stdbool.h>

#include "../nsk_wav_type.h"
#include "types/nsk_wav_ctx_meta.h"

/*!
 * \brief  Prints final decoded note/rest events.
 *
 * \param[in]      wav  Source WAV data.
 * \param[in,out]  ctx  Processing context containing decoded events.
 * \return True if decoded events were printed successfully.
 */
bool nsk_wav_output(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
);
