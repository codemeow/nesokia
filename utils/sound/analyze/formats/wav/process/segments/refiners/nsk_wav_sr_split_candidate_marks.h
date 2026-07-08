#pragma once

#include "../nsk_wav_sg_types.h"

/*!
 * \brief  Evaluates whether a candidate mark should split a segment.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in,out]  segment  Segment being analyzed or updated.
 * \param[in]  frame  Frame index or frame position.
 * \param[in,out]  left  Left segment or left numeric value.
 * \param[in]  right  Right segment or right numeric value.
 * \param[out]  use  Set to true when the candidate split should be applied.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_candidate_split(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    const struct nsk_wav_segment *segment,
    size_t frame,
    struct nsk_wav_segment *left,
    struct nsk_wav_segment *right,
    bool *use
);

/*!
 * \brief  Splits segments at accepted candidate marks when both sides remain valid.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[out]  changed  Set to true when the function modifies segment data.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_split_candidate_marks(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    bool *changed
);
