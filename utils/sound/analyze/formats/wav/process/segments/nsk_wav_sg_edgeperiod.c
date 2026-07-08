#include "nsk_wav_sg_edgeperiod.h"

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
) {
    *result = (struct _segment_pitch) {
        .found = false,
        .midi  = -1
    };

    const size_t samplecount = sampleend - samplestart;

    if (
        samplecount <
        nsk_options_program.profile.segments.edgeperiodminsamples
    ) {
        return true;
    }

    double peak = ctx->samples.value[samplestart];
    double trough = peak;

    for (size_t i = samplestart + 1; i < sampleend; i++) {
        peak = NSK_MAX(peak, ctx->samples.value[i]);
        trough = NSK_MIN(trough, ctx->samples.value[i]);
    }

    if (
        peak - trough <=
        nsk_options_program.profile.segments.edgeperiodp2pfloor
    ) {
        return true;
    }

    const double midline = (peak + trough) * 0.5;

    nsk_auto_free double *rising = calloc(samplecount, sizeof(*rising));
    nsk_auto_free double *falling = calloc(samplecount, sizeof(*falling));
    nsk_auto_free double *intervals = calloc(samplecount, sizeof(*intervals));

    if (!rising || !falling || !intervals) {
        nsk_err("Cannot allocate memory for segment edge periods");
        return false;
    }

    size_t risingcount = 0;
    size_t fallingcount = 0;

    double previous = ctx->samples.value[samplestart] - midline;

    for (size_t i = 1; i < samplecount; i++) {
        const double current =
            ctx->samples.value[samplestart + i] -
            midline;

        if (
            (previous <= 0.0 && current > 0.0) ||
            (previous < 0.0 && current >= 0.0)
        ) {
            const double denominator = current - previous;
            const double fraction =
                denominator == 0.0 ?
                0.0 :
                -previous / denominator;

            rising[risingcount++] =
                ((double)i - 1.0 + fraction) /
                wav->format.samplerate;

        } else if (
            (previous >= 0.0 && current < 0.0) ||
            (previous > 0.0 && current <= 0.0)
        ) {
            const double denominator = current - previous;
            const double fraction =
                denominator == 0.0 ?
                0.0 :
                -previous / denominator;

            falling[fallingcount++] =
                ((double)i - 1.0 + fraction) /
                wav->format.samplerate;
        }

        previous = current;
    }

    size_t intervalcount = 0;

    for (size_t i = 1; i < risingcount; i++) {
        intervals[intervalcount++] = rising[i] - rising[i - 1];
    }

    for (size_t i = 1; i < fallingcount; i++) {
        intervals[intervalcount++] = falling[i] - falling[i - 1];
    }

    if (intervalcount == 0) {
        return true;
    }

    double median = 0.0;

    if (!nsk_math_mediand(intervals, intervalcount, &median)) {
        return false;
    }

    if (median <= 0.0) {
        return true;
    }

    nsk_auto_free double *close = calloc(intervalcount, sizeof(*close));

    if (!close) {
        nsk_err("Cannot allocate memory for stable segment edge periods");
        return false;
    }

    size_t closecount = 0;

    for (size_t i = 0; i < intervalcount; i++) {
        if (
            fabs(intervals[i] - median) <=
            median *
            nsk_options_program.profile.segments.edgeperiodcloseperiodratio
        ) {
            close[closecount++] = intervals[i];
        }
    }

    const double *selected = intervals;
    size_t selectedcount = intervalcount;

    if (closecount >= 1) {
        selected = close;
        selectedcount = closecount;

        if (!nsk_math_mediand(selected, selectedcount, &median)) {
            return false;
        }

        if (median <= 0.0) {
            return true;
        }
    }

    double mean = 0.0;

    for (size_t i = 0; i < selectedcount; i++) {
        mean += selected[i];
    }

    mean /= selectedcount;

    double variance = 0.0;

    for (size_t i = 0; i < selectedcount; i++) {
        const double delta = selected[i] - mean;
        variance += delta * delta;
    }

    variance /= selectedcount;

    const double cv = sqrt(variance) / median;
    const double frequency = 1.0 / median;
    const int midi = nsk_midi_fromfreq(frequency);
    const double expected = nsk_midi_tofreq(midi);
    const double halfsemitone = pow(2.0, 1.0 / 24.0);
    const double freqlowest =
        nsk_options_program.profile.boundary.edgetrain.frequencylowest /
        halfsemitone;
    const double freqhighest =
        nsk_options_program.profile.boundary.edgetrain.frequencyhighest *
        halfsemitone;

    if (
        expected < freqlowest ||
        expected > freqhighest
    ) {
        return true;
    }

    const double cents = fabs(1200.0 * log2(frequency / expected));
    const double confidence =
        NSK_MAX(
            0.0,
            1.0 -
            cv *
            nsk_options_program.profile.segments.edgeperiodcvscale
        ) *
        NSK_MAX(
            0.0,
            1.0 -
            cents /
            nsk_options_program.profile.segments.edgeperiodcentsscale
        );

    *result = (struct _segment_pitch) {
        .found      = true,
        .midi       = midi,
        .frequency  = expected,
        .confidence = confidence,
        .support    = selectedcount
    };

    return true;
}

/*!
 * \brief  Creates an edge decision that leaves the spectral result unchanged.
 *
 * \return Constructed edge decision value.
 */
struct _segment_edge_decision nsk_wav_sg_edge_nodecision(void) {
    return (struct _segment_edge_decision) {
        .decided = false
    };
}

/*!
 * \brief  Creates an edge decision that replaces the pitch with the given MIDI note.
 *
 * \param[in]  midi  MIDI note value.
 * \return Constructed edge decision value.
 */
struct _segment_edge_decision nsk_wav_sg_edge_usedecision(int midi) {
    return (struct _segment_edge_decision) {
        .decided = true,
        .use     = true,
        .midi    = midi
    };
}

/*!
 * \brief  Creates an edge decision that blocks the given MIDI note.
 *
 * \param[in]  midi  MIDI note value.
 * \return Constructed edge decision value.
 */
struct _segment_edge_decision nsk_wav_sg_edge_blockdecision(int midi) {
    return (struct _segment_edge_decision) {
        .decided = true,
        .use     = false,
        .midi    = midi
    };
}

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
) {
    *decision = nsk_wav_sg_edge_nodecision();

    if (spectral->found) {
        return true;
    }

    *decision =
        edge->confidence >= threshold ?
        nsk_wav_sg_edge_usedecision(edge->midi) :
        nsk_wav_sg_edge_blockdecision(edge->midi);

    return true;
}

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
) {
    *decision = nsk_wav_sg_edge_nodecision();

    if (!spectral->found || spectral->midi != edge->midi) {
        return true;
    }

    *decision = nsk_wav_sg_edge_usedecision(edge->midi);

    return true;
}

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
) {
    *decision = nsk_wav_sg_edge_nodecision();

    if (!spectral->found || spectral->midi == edge->midi) {
        return true;
    }

    const int delta = edge->midi - spectral->midi;

    if (
        context->framecount <=
        nsk_options_program.profile.segments.cycleedgeoverridemaxframes &&
        (size_t)abs(delta) <=
        nsk_options_program.profile.segments.cycleedgeoverridemaxdeltast &&
        edge->support >=
        nsk_options_program.profile.segments.cycleedgeoverrideminsupport &&
        edge->confidence >=
        nsk_options_program.profile.segments.cycleedgeoverrideminconfidence
    ) {
        *decision = nsk_wav_sg_edge_usedecision(edge->midi);
    }

    return true;
}

/*!
 * \brief  Checks whether a high 12.5 percent duty candidate should be guarded.
 *
 * \param[in]  spectral  Spectral pitch result.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_edge_high125guard(
    const struct _segment_pitch *spectral
) {
    return
        spectral->found &&
        spectral->midi >=
        nsk_options_program.profile.segments.high125guardminmidi &&
        spectral->duty ==
        nsk_options_program.profile.segments.high125guardduty &&
        spectral->confidence >=
        nsk_options_program.profile.segments.high125guardminconfidence;
}

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
) {
    *decision = nsk_wav_sg_edge_nodecision();

    if (!spectral->found || spectral->midi == edge->midi) {
        return true;
    }

    if (nsk_wav_sg_edge_high125guard(spectral)) {
        return true;
    }

    if (
        edge->support <
        nsk_options_program.profile.segments.edgeperiodoctaveminsupport ||
        edge->confidence <
        nsk_options_program.profile.segments.edgeperiodoctaveminconfidence
    ) {
        return true;
    }

    const int delta = edge->midi - spectral->midi;

    if (delta == -(int)semitones_peroctave) {
        *decision = nsk_wav_sg_edge_usedecision(edge->midi);
        return true;
    }

    const int target = spectral->midi - (int)semitones_peroctave;

    if (
        target >= context->midimin &&
        abs(edge->midi - target) <= 1
    ) {
        *decision = nsk_wav_sg_edge_usedecision(target);
    }

    return true;
}

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
) {
    *decision = nsk_wav_sg_edge_nodecision();

    if (!spectral->found || spectral->midi == edge->midi) {
        return true;
    }

    const int delta = edge->midi - spectral->midi;
    const bool semitoneguard =
        abs(delta) == 1 &&
        spectral->confidence >=
        nsk_options_program.profile.segments.semitoneguardminconfidence &&
        edge->confidence < spectral->confidence;

    *decision =
        edge->confidence >= threshold && !semitoneguard ?
        nsk_wav_sg_edge_usedecision(edge->midi) :
        nsk_wav_sg_edge_blockdecision(edge->midi);

    return true;
}

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
) {
    /*!
     * \brief  Ordered edge-period decision rule table.
     */
    static const struct {
        _segment_edge_rule rule;
    } table[] = {
        {
            .rule = nsk_wav_sg_edge_rule_missing
        },
        {
            .rule = nsk_wav_sg_edge_rule_same
        },
        {
            .rule = nsk_wav_sg_edge_rule_cycle
        },
        {
            .rule = nsk_wav_sg_edge_rule_octavedown
        },
        {
            .rule = nsk_wav_sg_edge_rule_threshold
        }
    };

    *decision = nsk_wav_sg_edge_nodecision();

    if (!edge->found || edge->support < 1) {
        return true;
    }

    const double threshold =
        context->framecount <=
        nsk_options_program.profile.segments.edgeperiodshortmaxframes ?
        nsk_options_program.profile.segments.edgeperiodshortminconfidence :
        nsk_options_program.profile.segments.edgeperiodminconfidence;

    for (size_t i = 0; i < NSK_SIZE(table); i++) {
        if (!table[i].rule(context, spectral, edge, threshold, decision)) {
            return false;
        }

        if (decision->decided) {
            return true;
        }
    }

    return true;
}
