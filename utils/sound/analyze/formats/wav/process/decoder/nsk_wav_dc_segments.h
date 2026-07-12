#pragma once

#include <stdbool.h>

#include "nsk_wav_dc_marks.h"
#include "nsk_wav_dc_regions.h"
#include "nsk_wav_dc_sparse.h"
#include "../types/nsk_wav_ctx_meta.h"
#include "../../nsk_wav_type.h"

/*!
 * \brief  Builds a measured segment hypothesis from sparse decoder intervals.
 *
 * \param[in]      wav        Source WAV data
 * \param[in,out]  ctx        Processing context receiving segment hypothesis
 * \param[in]      intervals  Snapped sparse decoder intervals
 * \return True if segment hypothesis was built successfully
 */
bool nsk_wav_dc_segments_from_sparse(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx *ctx,
    const struct nsk_wav_dc_intervals *intervals
);

/*!
 * \brief  Builds a local-hybrid segment hypothesis from sparse and dense boundaries.
 *
 * \param[in]      wav        Source WAV data
 * \param[in,out]  ctx        Processing context receiving segment hypothesis
 * \param[in]      intervals  Snapped sparse decoder intervals
 * \param[in]      marks      Decoder mark evidence
 * \param[in]      regions    Local dense regions
 * \return True if segment hypothesis was built successfully
 */
bool nsk_wav_dc_segments_from_hybrid(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx *ctx,
    const struct nsk_wav_dc_intervals *intervals,
    const struct nsk_wav_dc_marks *marks,
    const struct nsk_wav_dc_regions *regions
);
