#pragma once

#include "nsk_wav_sg_types.h"

/*!
 * \brief  Scores a pulse template fit for a MIDI note, duty, and phase search.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  samplestart  Inclusive sample start.
 * \param[in]  sampleend  Exclusive sample end.
 * \param[in]  midi  MIDI note value.
 * \param[in]  duty  Pulse duty candidate.
 * \param[out]  fit  Calculated template fit.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_template_fit(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    size_t samplestart,
    size_t sampleend,
    int midi,
    double duty,
    struct _segment_templatefit *fit
);

/*!
 * \brief  Finds the best pulse template fit across MIDI and duty candidates.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  samplestart  Inclusive sample start.
 * \param[in]  sampleend  Exclusive sample end.
 * \param[in,out]  midis  MIDI candidate array.
 * \param[in,out]  midicount  Number of MIDI candidates.
 * \param[out]  best  Best calculated template fit.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_best_template_fit(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    size_t samplestart,
    size_t sampleend,
    const int *midis,
    size_t midicount,
    struct _segment_templatefit *best
);

/*!
 * \brief  Adds a MIDI candidate to a candidate list if it is not already present.
 *
 * \param[in,out]  midis  MIDI candidate array.
 * \param[in,out]  count  Number of used items.
 * \param[in]  capacity  Maximum number of items available in the destination array.
 * \param[in]  midi  MIDI note value.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_add_candidate_midi(
    int *midis,
    size_t *count,
    size_t capacity,
    int midi
);

/*!
 * \brief  Builds the MIDI candidate set for template fitting.
 *
 * \param[in]  primary  Primary MIDI candidate.
 * \param[in]  edge  Edge-period MIDI candidate or pitch result.
 * \param[in]  period  Period-scoring MIDI candidate.
 * \param[in,out]  framecount  Number of frames in the analyzed window or repair group.
 * \param[in,out]  midis  MIDI candidate array.
 * \param[in,out]  midicount  Number of MIDI candidates.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_template_candidates(
    int primary,
    int edge,
    int period,
    size_t framecount,
    int **midis,
    size_t *midicount
);

/*!
 * \brief  Applies template-fit override logic to a single segment.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in,out]  segment  Segment being analyzed or updated.
 * \param[out]  changed  Set to true when the function modifies segment data.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_segment_template_override(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    struct nsk_wav_segment *segment,
    bool *changed
);
