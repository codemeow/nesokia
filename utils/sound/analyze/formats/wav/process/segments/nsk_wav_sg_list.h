#pragma once

#include "nsk_wav_sg_types.h"

/*!
 * \brief  Converts a frame position to the nearest source sample index.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  frame  Frame index or frame position.
 * \return Calculated size value.
 */
size_t nsk_wav_sg_frame_sample(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    double frame
);

/*!
 * \brief  Appends a segment described by frame bounds to the segment list.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  framestart  Inclusive frame start.
 * \param[in]  frameend  Exclusive frame end.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_appendframes(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    size_t          framestart,
    size_t          frameend
);

/*!
 * \brief  Appends a segment covering a contiguous accepted mark range.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  start  Inclusive mark range start.
 * \param[in]  end  Exclusive mark range end.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_append_mark_range(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    size_t          start,
    size_t          end
);

/*!
 * \brief  Builds the initial segment list from accepted marks.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_create(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
);

/*!
 * \brief  Creates a segment value from frame bounds.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  framestart  Inclusive frame start.
 * \param[in]  frameend  Exclusive frame end.
 * \return Constructed segment value.
 */
struct nsk_wav_segment nsk_wav_sg_fromframes(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    size_t framestart,
    size_t frameend
);

/*!
 * \brief  Replaces a range of segments with the supplied segment items.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  index  First segment index to replace or repair.
 * \param[in]  removecount  Number of existing segments to remove.
 * \param[in]  items  Replacement segment array.
 * \param[in]  itemcount  Number of replacement items.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_replace_range(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    size_t index,
    size_t removecount,
    const struct nsk_wav_segment *items,
    size_t itemcount
);

/*!
 * \brief  Checks whether two neighboring segments are equal for merge purposes.
 *
 * \param[in,out]  left  Left segment or left numeric value.
 * \param[in]  right  Right segment or right numeric value.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_isequal(
    const struct nsk_wav_segment *left,
    const struct nsk_wav_segment *right
);

/*!
 * \brief  Calculates a weighted average for two segment measurements.
 *
 * \param[in,out]  left  Left segment or left numeric value.
 * \param[in]  leftweight  Weight for the left value.
 * \param[in]  right  Right segment or right numeric value.
 * \param[in]  rightweight  Weight for the right value.
 * \return Calculated floating-point value.
 */
double nsk_wav_sg_weightedaverage(
    double left,
    size_t leftweight,
    double right,
    size_t rightweight
);

/*!
 * \brief  Merges the right segment into the left segment.
 *
 * \param[in,out]  left  Left segment or left numeric value.
 * \param[in]  right  Right segment or right numeric value.
 */
void nsk_wav_sg_mergeitem(
    struct nsk_wav_segment *left,
    const struct nsk_wav_segment *right
);

/*!
 * \brief  Merges adjacent equivalent segments in place.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_merge(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
);
