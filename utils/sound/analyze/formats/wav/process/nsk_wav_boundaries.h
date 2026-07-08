#pragma once

#include <stdbool.h>

#include "../nsk_wav_type.h"
#include "types/nsk_wav_ctx_meta.h"

/*!
 * \brief  Finds the boundaries between notes
 *
 * Runs all configured boundary detectors and accumulates candidates in the
 * processing context.
 *
 * \param[in]      wav  Source WAV data
 * \param[in,out]  ctx  Processing context
 * \return True if all boundary detectors completed successfully
 */
bool nsk_wav_boundaries(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
);
