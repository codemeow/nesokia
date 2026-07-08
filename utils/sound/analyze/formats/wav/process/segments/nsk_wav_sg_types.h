#pragma once

#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <string.h>

#include <nsk_util_meta.h>

#include "../nsk_wav_segments.h"
#include "../../../../arguments/nsk_args_options.h"
#include "../../nsk_wav_midi.h"

/*!
 * Number of semitones per octave
 */
static const size_t semitones_peroctave = 12;

/*!
 * \brief  Local pitch estimate and scorer result.
 */
struct _segment_pitch {
    bool   found;
    int    midi;
    double frequency;
    double confidence;
    size_t support;
    double ratio;
    double duty;
    double score;
};

/*!
 * \brief  Time-domain pulse template fitting result.
 */
struct _segment_templatefit {
    bool   found;
    int    midi;
    double duty;
    double score;
    double residualratio;
    double phase;
    double gain;
    double dc;
};

/*!
 * \brief  One-frame pulse-width pitch evidence.
 */
struct _segment_pulsewidth {
    bool   found;
    int    midi;
    double confidence;
    size_t support;
    double duty;
    double templatescore;
    double templatemargin;
};

/*!
 * \brief  Context shared by edge-period decision rules.
 */
struct _segment_edge_context {
    size_t framecount;
    int    midimin;
};

/*!
 * \brief  Decision returned by an edge-period rule.
 */
struct _segment_edge_decision {
    bool decided;
    bool use;
    int  midi;
};

/*!
 * \brief  Edge-period decision rule callback.
 *
 * \param[in]  context    Segment-level decision context
 * \param[in]  spectral   Spectral pitch estimate
 * \param[in]  edge       Edge-period pitch estimate
 * \param[in]  threshold  Minimum confidence for regular edge usage
 * \param[out] decision   Rule decision
 * \return True if the rule completed successfully
 */
typedef bool (*_segment_edge_rule)(
    const struct _segment_edge_context *context,
    const struct _segment_pitch *spectral,
    const struct _segment_pitch *edge,
    double threshold,
    struct _segment_edge_decision *decision
);

/*!
 * \brief  NES pulse duty candidates, in percent.
 */
static const double _segment_duties[] = {
    12.5,
    25.0,
    50.0,
    75.0
};

