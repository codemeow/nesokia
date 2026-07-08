#pragma once

#include "nsk_wav_sg_types.h"

/*!
 * \brief  Calculates peak-to-peak amplitude for a single frame.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  frame  Frame index or frame position.
 * \return Calculated floating-point value.
 */
double nsk_wav_sg_frame_p2p(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    size_t frame
);

/*!
 * \brief  Calculates the global silence threshold from frame peak-to-peak values.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_calcsilence(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
);

/*!
 * \brief  Calculates peak-to-peak amplitude for a segment.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in,out]  segment  Segment being analyzed or updated.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_p2p(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    struct nsk_wav_segment *segment
);

/*!
 * \brief  Calculates normalized volume for a segment.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in,out]  segment  Segment being analyzed or updated.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_volume(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    struct nsk_wav_segment *segment
);

/*!
 * \brief  Classifies a segment as active sound or rest.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in,out]  segment  Segment being analyzed or updated.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_active(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    struct nsk_wav_segment *segment
);

/*!
 * \brief  Calculates peak-to-peak amplitude delta around a frame boundary.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  frame  Frame index or frame position.
 * \return Calculated floating-point value.
 */
double nsk_wav_sg_mark_p2p_delta(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    size_t frame
);

/*!
 * \brief  Estimates folded pulse duty from the full segment waveform.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in,out]  segment  Segment being analyzed or updated.
 * \param[in]  duty  Pulse duty candidate.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_estimate_folded_duty(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    const struct nsk_wav_segment *segment,
    double *duty
);
