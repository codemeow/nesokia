#pragma once

#include <stddef.h>

#include "../types/nsk_wav_ctx_meta.h"

/*!
 * \brief  Scores the currently selected segment hypothesis.
 *
 * \param[in] ctx       Processing context containing the segment hypothesis.
 * \param[in] maxframe  One-past-last decoded frame.
 * \return Hypothesis score; larger values are better.
 */
double nsk_wav_dc_score_segments(
    const struct nsk_wav_ctx *ctx,
    size_t maxframe
);
