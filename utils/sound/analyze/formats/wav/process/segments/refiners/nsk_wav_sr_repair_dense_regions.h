#pragma once

#include "../nsk_wav_sg_types.h"

/*!
 * \brief  Checks whether MIDI and duration arrays match a dense chromatic repair shape.
 *
 * \param[in,out]  midis  MIDI candidate array.
 * \param[in]  durations  Frame durations for the candidate repair group.
 * \param[in]  itemcount  Number of replacement items.
 * \param[in,out]  framecount  Number of frames in the analyzed window or repair group.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_dense_chromatic_shape(
    int    *midis,
    size_t *durations,
    size_t  itemcount,
    size_t *framecount
);

/*!
 * \brief  Creates one dense chromatic repair segment item.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  framestart  Inclusive frame start.
 * \param[in]  frameend  Exclusive frame end.
 * \param[in]  midi  MIDI note value.
 * \param[in,out]  segment  Segment being analyzed or updated.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_dense_chromatic_item(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    size_t framestart,
    size_t frameend,
    int midi,
    struct nsk_wav_segment *segment
);

/*!
 * \brief  Repairs a dense chromatic segment group at a given segment index.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  index  First segment index to replace or repair.
 * \param[in]  removecount  Number of existing segments to remove.
 * \param[in]  framestart  Inclusive frame start.
 * \param[out]  changed  Set to true when the function modifies segment data.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_repair_dense_chromatic_at(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    size_t index,
    size_t removecount,
    size_t framestart,
    bool *changed
);

/*!
 * \brief  Searches the segment list for dense chromatic repair opportunities.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[out]  changed  Set to true when the function modifies segment data.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_repair_dense_chromatic_sweep(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    bool *changed
);

/*!
 * \brief  Repairs dense regions using dense-specific reclassification rules.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[out]  changed  Set to true when the function modifies segment data.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_repair_dense_regions(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    bool *changed
);
