#pragma once

#include "nsk_wav_sg_types.h"

/*!
 * \brief  Segment hypothesis processing options.
 */
struct nsk_wav_sg_pipeline {
    bool merge;       /*!< Whether regular equal-neighbor merging is allowed */
    bool refiners;    /*!< Whether segment refiners should run */
    bool densedirect; /*!< Whether dense-direct merge suppression is active */
};

/*!
 * \brief  Processes the current segment list as one hypothesis.
 *
 * \param[in]      wav      Source WAV data.
 * \param[in,out]  ctx      Processing context used by the segment pipeline.
 * \param[in]      options  Segment pipeline options.
 * \return True if the hypothesis was processed successfully.
 */
bool nsk_wav_sg_process(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    const struct nsk_wav_sg_pipeline *options
);

/*!
 * \brief  Builds and processes a segment hypothesis from accepted marks.
 *
 * \param[in]      wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \return True if the hypothesis was built successfully.
 */
bool nsk_wav_sg_build_marks(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
);

/*!
 * \brief  Builds and processes a segment hypothesis from explicit boundaries.
 *
 * \param[in]      wav         Source WAV data.
 * \param[in,out]  ctx         Processing context used by the segment pipeline.
 * \param[in]      boundaries  Ordered frame boundary list.
 * \param[in]      count       Number of frame boundaries.
 * \param[in]      options     Segment pipeline options.
 * \return True if the hypothesis was built successfully.
 */
bool nsk_wav_sg_build_boundaries(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    const size_t *boundaries,
    size_t count,
    const struct nsk_wav_sg_pipeline *options
);
