#pragma once

#include <stdbool.h>

#include "../../nsk_wav_type.h"
#include "../types/nsk_wav_ctx_meta.h"

/*!
 * \brief  Finds the boundaries by edge trains
 *
 * Same-polarity edge trains describe full waveform periods without depending
 * on pulse duty.  This stage processes rising and falling trains separately,
 * builds adjacent-edge periods for each train, and compares stable period
 * windows around every possible mixed interval.
 *
 * When the periods before and after such an interval are stable and their
 * pitch distance is large enough, the stage emits period-change candidates.
 * These candidates are evidence that a musical boundary is nearby; later grid
 * and decoder stages decide whether they become accepted note boundaries.
 *
 * \param[in]      wav  Source WAV data
 * \param[in,out]  ctx  Processing context containing spans and edges
 * \return True if all edges were processed successfully
 */
bool nsk_wav_bf_edgetrains(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
);
