#pragma once

#include "nsk_wav_sg_types.h"

/*!
 * \brief  Estimates pitch from edge-period timing inside a sample range.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  samplestart  Inclusive sample start.
 * \param[in]  sampleend  Exclusive sample end.
 * \param[out]  result  Calculated result.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_edgeperiod(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    size_t samplestart,
    size_t sampleend,
    struct _segment_pitch *result
);

/*!
 * \brief  Creates an edge decision that leaves the spectral result unchanged.
 *
 * \return Constructed edge decision value.
 */
struct _segment_edge_decision nsk_wav_sg_edge_nodecision(void);

/*!
 * \brief  Creates an edge decision that replaces the pitch with the given MIDI note.
 *
 * \param[in]  midi  MIDI note value.
 * \return Constructed edge decision value.
 */
struct _segment_edge_decision nsk_wav_sg_edge_usedecision(int midi);

/*!
 * \brief  Creates an edge decision that blocks the given MIDI note.
 *
 * \param[in]  midi  MIDI note value.
 * \return Constructed edge decision value.
 */
struct _segment_edge_decision nsk_wav_sg_edge_blockdecision(int midi);

/*!
 * \brief  Applies the edge decision rule for missing or weak spectral evidence.
 *
 * \param[in]  context  Edge decision context.
 * \param[in]  spectral  Spectral pitch result.
 * \param[in]  edge  Edge-period MIDI candidate or pitch result.
 * \param[in]  threshold  Decision score threshold.
 * \param[out]  decision  Selected edge decision.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_edge_rule_missing(
    const struct _segment_edge_context *context __attribute__((unused)),
    const struct _segment_pitch *spectral,
    const struct _segment_pitch *edge,
    double threshold,
    struct _segment_edge_decision *decision
);

/*!
 * \brief  Applies the edge decision rule for matching spectral and edge notes.
 *
 * \param[in]  context  Edge decision context.
 * \param[in]  spectral  Spectral pitch result.
 * \param[in]  edge  Edge-period MIDI candidate or pitch result.
 * \param[in]  threshold  Decision score threshold.
 * \param[out]  decision  Selected edge decision.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_edge_rule_same(
    const struct _segment_edge_context *context __attribute__((unused)),
    const struct _segment_pitch *spectral,
    const struct _segment_pitch *edge,
    double threshold __attribute__((unused)),
    struct _segment_edge_decision *decision
);

/*!
 * \brief  Applies the edge decision rule for cycle-related pitch candidates.
 *
 * \param[in]  context  Edge decision context.
 * \param[in]  spectral  Spectral pitch result.
 * \param[in]  edge  Edge-period MIDI candidate or pitch result.
 * \param[in]  threshold  Decision score threshold.
 * \param[out]  decision  Selected edge decision.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_edge_rule_cycle(
    const struct _segment_edge_context *context,
    const struct _segment_pitch *spectral,
    const struct _segment_pitch *edge,
    double threshold __attribute__((unused)),
    struct _segment_edge_decision *decision
);

/*!
 * \brief  Checks whether a high 12.5 percent duty candidate should be guarded.
 *
 * \param[in]  spectral  Spectral pitch result.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_edge_high125guard(
    const struct _segment_pitch *spectral
);

/*!
 * \brief  Applies the edge decision rule for octave-down corrections.
 *
 * \param[in]  context  Edge decision context.
 * \param[in]  spectral  Spectral pitch result.
 * \param[in]  edge  Edge-period MIDI candidate or pitch result.
 * \param[in]  threshold  Decision score threshold.
 * \param[out]  decision  Selected edge decision.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_edge_rule_octavedown(
    const struct _segment_edge_context *context,
    const struct _segment_pitch *spectral,
    const struct _segment_pitch *edge,
    double threshold __attribute__((unused)),
    struct _segment_edge_decision *decision
);

/*!
 * \brief  Applies the edge decision rule based on the supplied score threshold.
 *
 * \param[in]  context  Edge decision context.
 * \param[in]  spectral  Spectral pitch result.
 * \param[in]  edge  Edge-period MIDI candidate or pitch result.
 * \param[in]  threshold  Decision score threshold.
 * \param[out]  decision  Selected edge decision.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_edge_rule_threshold(
    const struct _segment_edge_context *context __attribute__((unused)),
    const struct _segment_pitch *spectral,
    const struct _segment_pitch *edge,
    double threshold,
    struct _segment_edge_decision *decision
);

/*!
 * \brief  Selects the final edge-period decision from the ordered rule set.
 *
 * \param[in]  context  Edge decision context.
 * \param[in]  spectral  Spectral pitch result.
 * \param[in]  edge  Edge-period MIDI candidate or pitch result.
 * \param[out]  decision  Selected edge decision.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_edgedecide(
    const struct _segment_edge_context *context,
    const struct _segment_pitch *spectral,
    const struct _segment_pitch *edge,
    struct _segment_edge_decision *decision
);
