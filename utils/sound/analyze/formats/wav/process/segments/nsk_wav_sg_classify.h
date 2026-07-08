#pragma once

#include "nsk_wav_sg_types.h"

/*!
 * \brief  Calculates the trimmed sample bounds used for segment classification.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in,out]  segment  Segment being analyzed or updated.
 * \param[in]  samplestart  Inclusive sample start.
 * \param[in]  sampleend  Exclusive sample end.
 */
void nsk_wav_sg_corebounds(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    const struct nsk_wav_segment *segment,
    size_t *samplestart,
    size_t *sampleend
);

/*!
 * \brief  Classifies a segment pitch and duty from its spectral evidence.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in,out]  segment  Segment being analyzed or updated.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_classify(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    struct nsk_wav_segment *segment
);

/*!
 * \brief  Recomputes all derived measurements and classification for a segment.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in,out]  segment  Segment being analyzed or updated.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_recalc(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    struct nsk_wav_segment *segment
);
