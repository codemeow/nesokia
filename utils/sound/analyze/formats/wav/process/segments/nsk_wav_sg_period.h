#pragma once

#include "nsk_wav_sg_types.h"

/*!
 * \brief  Estimates pitch from autocorrelation period scoring.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  samplestart  Inclusive sample start.
 * \param[in]  sampleend  Exclusive sample end.
 * \param[out]  pitch  Calculated pitch result.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_period_note(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    size_t samplestart,
    size_t sampleend,
    struct _segment_pitch *pitch
);
