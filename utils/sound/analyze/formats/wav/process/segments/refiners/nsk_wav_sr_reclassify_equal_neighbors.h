#pragma once

#include "../nsk_wav_sg_types.h"

/*!
 * \brief  Reclassifies a run of equal neighboring segments as one analysis window.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[out]  changed  Set to true when the function modifies segment data.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_reclassify_equal_neighbors(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    bool *changed
);
