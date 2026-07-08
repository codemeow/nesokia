#pragma once

#include <stdbool.h>

#include "../nsk_wav_type.h"
#include "types/nsk_wav_ctx_meta.h"

/*!
 * \brief  Creates mark pools based on the candidates
 *
 * Boundary candidates are continuous-time events.  This stage maps them onto
 * the target frame grid by rounding each candidate timestamp to the nearest
 * frame and checking that the timing error is within the configured window.
 *
 * The resulting mark array is dense: every internal frame has a mark slot, and
 * each slot contains the candidate pointers that landed near that frame.  The
 * later selector/decoder stages can then reason in integer frame coordinates
 * without losing the original candidate timestamps.
 *
 * \param[in]      wav  Source WAV data
 * \param[in,out]  ctx  Processing context receiving quantized marks
 * \return True if quantization completed successfully
 */
bool nsk_wav_quantization(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
);
