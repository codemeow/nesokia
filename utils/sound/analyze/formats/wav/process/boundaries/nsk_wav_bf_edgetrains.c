#include <math.h>

#include <nsk_util_meta.h>

#include "nsk_wav_bf_edgetrains.h"
#include "../../../../arguments/nsk_args_options.h"


/*!
 * \brief Number of semitones in one octave
 */
static const size_t _semitones_peroctave = 12;

/*!
 * \brief  Returns the representative period for a local edge-train window
 *
 * The same-polarity edge train converts adjacent edge timestamps into full
 * waveform periods.  Around a suspected transition, the algorithm compares a
 * small window of periods before the transition with a small window after it.
 *
 * This helper reduces such a period window to one robust representative value.
 * The median is used instead of the arithmetic mean so that a single shifted
 * edge has limited influence on the local pitch estimate.
 *
 * \param[in]  periods  Period window
 * \param[in]  count    Number of periods in the window
 * \param[out] period   Median period for the window
 * \return     True if the representative period was calculated
 */
static bool _bf_edgetrains_robust_period(
    const double *periods,
    size_t        count,
    double       *period
) {
    return nsk_math_mediand(
        periods,
        count,
        period
    );
}

/*!
 * \brief  Estimates local period stability for an edge-train window
 *
 * The period evidence is useful only when the periods on each side of a
 * suspected transition describe a stable waveform.  This helper calculates a
 * robust coefficient of variation for one period window:
 *
 *     median(abs(periods - median(periods))) / median(periods)
 *
 * Values near zero mean that the periods in the window agree closely.  Larger
 * values mean that the local edge train is too irregular for reliable
 * left/right pitch comparison.
 *
 * \param[in] periods  Period window
 * \param[in] count    Number of periods in the window
 * \return    Robust coefficient of variation
 */
static double _bf_edgetrains_period_cv(
    const double *periods,
    size_t        count
) {
    double median = 0.0;

    if (!_bf_edgetrains_robust_period(periods, count, &median)) {
        return INFINITY;
    }

    if (count <= 1) {
        return 0.0;
    }

    nsk_auto_free double *deviation = calloc(
        count,
        sizeof(*deviation)
    );
    if (!deviation) {
        nsk_err(
            "Cannot allocate memory for period deviations"
        );
        return INFINITY;
    }

    for (size_t i = 0; i < count; i++) {
        deviation[i] = fabs(periods[i] - median);
    }

    double mad = 0.0;
    if (!nsk_math_mediand(deviation, count, &mad)) {
        return INFINITY;
    }

    return mad / median;
}

/*!
 * \brief  Maps local period stability to candidate confidence
 *
 * Stable left and right period windows produce confidence close to 1.0.
 * Larger period variation lowers confidence before method-specific scaling is
 * applied by the candidate builder.
 *
 * \param[in] leftcv   Stability estimate for periods before transition
 * \param[in] rightcv  Stability estimate for periods after transition
 * \return    Base confidence before method-specific scaling
 */
static double _bf_edgetrains_confidence(
    double leftcv,
    double rightcv
) {
    return 1.0 /
        (1.0 + _semitones_peroctave * NSK_MAX(leftcv, rightcv));
}

/*!
 * \brief  Adds a weak candidate at the observed edge after a period change
 *
 * When stable periods on the left and right sides differ, the first measured
 * same-polarity edge after the suspicious interval is useful evidence that a
 * transition happened nearby.
 *
 * The confidence is derived from the local period stability.  The weak factor
 * keeps this candidate below stronger period solvers because the true musical
 * boundary may lie inside the interval before the observed edge.
 *
 * \param[in,out] wav         The wav
 * \param[in]     timestamp   Candidate time in seconds
 * \param[in]     leftcv      Stability estimate for periods before transition
 * \param[in]     rightcv     Stability estimate for periods after transition
 * \param[in]     deltast     Pitch distance in semitones
 * \param[in]     shortperiod True if the short period candidate was added beforehand
 * \return        True if the candidate was added successfully
 */
static bool _bf_edgetrains_addedgechange(
    struct nsk_wav *wav,
    double timestamp,
    double leftcv,
    double rightcv,
    double deltast,
    bool   shortperiod
) {
    if (shortperiod) {
        return true;
    }

    const double confidence =
        _bf_edgetrains_confidence(leftcv, rightcv) *
        nsk_options_program.profile.boundary.edgetrain.confidencescalepec;

    const double strength = fabs(deltast) * confidence;

    return nsk_wav_candidate(
        wav,
        (struct nsk_wav_candidate) {
            .method     = NSK_WAV_CND_METHOD_EDGE_CHANGE,
            .kind       = NSK_WAV_CND_KIND_EDGE,
            .timestamp  = timestamp,
            .strength   = strength,
            .confidence = confidence,
            .deltast    = deltast
        }
    );
}

/*!
 * \brief  Adds weak probes inside a suspicious transition interval
 *
 * When stable periods on both sides indicate a pitch change, the actual
 * musical boundary may be located anywhere between the two same-polarity
 * edges that form the mixed interval.  This helper places weak candidates at
 * fixed fractions inside that interval.
 *
 * \param[in,out] wav         The wav
 * \param[in]     timestampl  Left edge of the mixed interval, in seconds
 * \param[in]     timestampr  Right edge of the mixed interval, in seconds
 * \param[in]     leftcv      Stability estimate before transition
 * \param[in]     rightcv     Stability estimate after transition
 * \param[in]     deltast     Pitch distance in semitones
 * \return        True if all probe candidates were added successfully
 */
static bool _bf_edgetrains_addtransitionprobes(
    struct nsk_wav *wav,
    double timestampl,
    double timestampr,
    double leftcv,
    double rightcv,
    double deltast
) {
    static const struct {
        double fraction;
    } _table[] = {
        { .fraction = 0.25 },
        { .fraction = 0.50 },
        { .fraction = 0.75 }
    };

    const double confidence =
        _bf_edgetrains_confidence(leftcv, rightcv) *
        nsk_options_program.profile.boundary.edgetrain.confidencescaletp;

    const double strength = fabs(deltast) * confidence;

    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        const double timestamp =
            timestampl + (timestampr - timestampl) * _table[i].fraction;

        if (!nsk_wav_candidate(
            wav,
            (struct nsk_wav_candidate) {
                .method     = NSK_WAV_CND_METHOD_TRANSITION_PROBES,
                .kind       = NSK_WAV_CND_KIND_TRANSITION_PROBE,
                .timestamp  = timestamp,
                .strength   = strength,
                .confidence = confidence,
                .deltast    = deltast
            }
        )) {
            return false;
        }
    }

    return true;
}

/*!
 * \brief  Adds an analytically solved candidate inside a mixed period
 *
 * A same-polarity period can contain the tail of one note and the beginning
 * of the next one.  In that case the boundary is not necessarily located on
 * either observed edge.  This helper solves the linear phase equation for the
 * transition time inside the mixed interval.
 *
 * The helper quietly skips intervals that cannot be solved or whose solution
 * falls outside the interval.  It reports failure only when a valid candidate
 * could not be appended.
 *
 * \param[in,out] wav             The wav
 * \param[in]     leftedge        Left edge of the mixed interval, in seconds
 * \param[in]     rightedge       Right edge of the mixed interval, in seconds
 * \param[in]     leftperiod      Period before transition, in seconds
 * \param[in]     rightperiod     Period after transition, in seconds
 * \param[in]     leftcv          Stability estimate before transition
 * \param[in]     rightcv         Stability estimate after transition
 * \param[in]     deltast         Pitch distance in semitones
 * \return        True if processing can continue
 */
static bool _bf_edgetrains_addmixedsolve(
    struct nsk_wav *wav,
    double leftedge,
    double rightedge,
    double leftperiod,
    double rightperiod,
    double leftcv,
    double rightcv,
    double deltast
) {
    static const double frequency_mindiff = 1e-12;

    const double leftfrequency = 1.0 / leftperiod;
    const double rightfrequency = 1.0 / rightperiod;
    const double denominator = leftfrequency - rightfrequency;
    if (fabs(denominator) <= frequency_mindiff) {
        return true;
    }

    const double tau =
        (1.0 - rightedge / rightperiod + leftedge / leftperiod) /
        denominator;

    if (tau < leftedge || tau > rightedge) {
        return true;
    }

    const double confidence = _bf_edgetrains_confidence(leftcv, rightcv);
    const double strength = fabs(deltast) * confidence;

    return nsk_wav_candidate(
        wav,
        (struct nsk_wav_candidate) {
            .method     = NSK_WAV_CND_METHOD_MIXED_PERIOD_SOLVE,
            .kind       = NSK_WAV_CND_KIND_LEGATO,
            .timestamp  = tau,
            .strength   = strength,
            .confidence = confidence,
            .deltast    = deltast
        }
    );
}

/*!
 * \brief  Adds a reset-like candidate for an abnormally short mixed period
 *
 * A note change can reset waveform phase.  In that case the same-polarity
 * interval that crosses the transition may be much shorter than the stable
 * periods observed on both sides.  Such an interval is treated as reset-like,
 * and the candidate is placed on the next observed same-polarity edge.
 *
 * The reset boost increases strength according to how much shorter the mixed
 * period is than the stable local period.
 *
 * \param[in,out] wav          The wav
 * \param[in]     rightedge    Edge after the mixed interval, in seconds
 * \param[in]     leftperiod   Representative period before transition
 * \param[in]     rightperiod  Representative period after transition
 * \param[in]     mixedperiod  Period of the suspicious mixed interval
 * \param[in]     leftcv       Stability estimate before transition
 * \param[in]     rightcv      Stability estimate after transition
 * \param[in]     deltast      Pitch distance in semitones
 * \param[out]    added        True when a short-period candidate was emitted
 * \return        True if processing can continue
 */
static bool _bf_edgetrains_addshortperiod(
    struct nsk_wav  *wav,
    double           rightedge,
    double           leftperiod,
    double           rightperiod,
    double           mixedperiod,
    double           leftcv,
    double           rightcv,
    double           deltast,
    bool            *added
) {
    *added = false;

    const double stableperiod = NSK_MIN(
        leftperiod,
        rightperiod
    );

    if (
        mixedperiod >=
        stableperiod *
            nsk_options_program.profile.boundary.edgetrain.shortresetratio
    ) {
        return true;
    }

    const double confidence = _bf_edgetrains_confidence(leftcv, rightcv);
    const double strength = (
        fabs(deltast) +
        fabs(log(mixedperiod  / stableperiod))
    ) * confidence;

    *added = true;

    return nsk_wav_candidate(
        wav,
        (struct nsk_wav_candidate) {
            .method     = NSK_WAV_CND_METHOD_SHORT_PERIOD_EDGE,
            .kind       = NSK_WAV_CND_KIND_WAVEFORM_RESET,
            .timestamp  = rightedge,
            .strength   = strength,
            .confidence = confidence,
            .deltast    = deltast
        }
    );
}

/*!
 * \brief  Finds the edge range inside one active span
 *
 * Edge timestamps are stored globally in seconds, while active spans are
 * stored in samples.  The returned range is half-open and contains only edges
 * strictly inside the active span.
 *
 * \param[in]  edge       The edge list
 * \param[in]  spanstart  Active span start in seconds
 * \param[in]  spanend    Active span end in seconds
 * \param[out] first      First edge index inside the span
 * \param[out] last       One-past-last edge index inside the span
 */
static void _bf_edgetrains_spanedgerange(
    const struct nsk_wav_edges *edge,
    double                      spanstart,
    double                      spanend,
    size_t                     *first,
    size_t                     *last
) {
    *first = 0;

    while (
        *first < edge->count &&
        edge->edge[*first] <= spanstart
    ) {
        (*first)++;
    }

    *last = *first;

    while (
        *last < edge->count &&
        edge->edge[*last] < spanend
    ) {
        (*last)++;
    }
}

/*!
 * \brief  Checks if the frequency in the supported range
 *
 * \param[in] frequency  The frequency
 * \return    True if the frequence is in range
 */
static bool _bf_edgetrains_inrange(
    double frequency
) {
    const double halfsemitone = pow(2.0, 1.0 / 24.0);

    const double lowest =
        nsk_options_program.profile.boundary.edgetrain.frequencylowest /
        halfsemitone;

    const double highest =
        nsk_options_program.profile.boundary.edgetrain.frequencyhighest *
        halfsemitone;

    return
        frequency >= lowest &&
        frequency <= highest;
}


/*!
 * \brief  Processes transition candidates inside one span edge range
 *
 * \param[in] wav   The wav
 * \param[in] edge  The edge list
 * \param[in] first First edge index inside the span
 * \param[in] last  One-past-last edge index inside the span
 * \return  True if successfully processed
 */
static bool _bf_edgetrains_process_transitioncandidates(
    struct nsk_wav             *wav,
    const struct nsk_wav_edges *edge,
    size_t                      first,
    size_t                      last
) {
    const size_t count = last - first;
    const size_t mincontext =
        /* Left context periods */
        nsk_options_program.profile.boundary.edgetrain.periodcontext +
        /* Center (mixed) point */
        1 +
        /* Right context periods */
        nsk_options_program.profile.boundary.edgetrain.periodcontext +
        /* Number of periods are 1 less than edge count */
        1;

    if (count < mincontext) {
        /* The edge train is too small */
        return true;
    }

    nsk_auto_free double *periods = calloc(
        count - 1,
        sizeof(*periods)
    );
    if (!periods) {
        nsk_err(
            "Cannot allocate memory for the periods list"
        );
        return false;
    }

    for (size_t i = 0; i < count - 1; i++) {
        periods[i] =
            edge->edge[first + i + 1] -
            edge->edge[first + i];
    }

    const size_t context =
        nsk_options_program.profile.boundary.edgetrain.periodcontext;

    for (size_t mixed = context; mixed < count - 1 - context; mixed++) {
        const double *leftperiods = &periods[mixed - context];
        const double *rightperiods = &periods[mixed + 1];

        const double leftedge  = edge->edge[first + mixed];
        const double rightedge = edge->edge[first + mixed + 1];

        double leftperiod = 0.0;
        double rightperiod = 0.0;

        if (!_bf_edgetrains_robust_period(
            leftperiods,
            context,
            &leftperiod
        )) {
            continue;
        }

        if (!_bf_edgetrains_robust_period(
            rightperiods,
            context,
            &rightperiod
        )) {
            continue;
        }

        const double leftcv = _bf_edgetrains_period_cv(
            leftperiods,
            context
        );

        const double rightcv = _bf_edgetrains_period_cv(
            rightperiods,
            context
        );

        if (
            NSK_MAX(leftcv, rightcv) >
            nsk_options_program.profile.boundary.edgetrain.maxperiodcv
        ) {
            continue;
        }

        const double leftfrequency = 1.0 / leftperiod;
        const double rightfrequency = 1.0 / rightperiod;

        if (!_bf_edgetrains_inrange(leftfrequency) ||
            !_bf_edgetrains_inrange(rightfrequency)) {
            continue;
        }

        const double deltast =
            _semitones_peroctave *
            log2(
                rightfrequency / leftfrequency
            );

        if (
            fabs(deltast) <
            nsk_options_program.profile.boundary.edgetrain.mindeltast
        ) {
            continue;
        }

        if (!_bf_edgetrains_addtransitionprobes(
            wav,
            leftedge,
            rightedge,
            leftcv,
            rightcv,
            deltast
        )) {
            return false;
        }

        if (!_bf_edgetrains_addmixedsolve(
            wav,
            leftedge,
            rightedge,
            leftperiod,
            rightperiod,
            leftcv,
            rightcv,
            deltast
        )) {
            return false;
        }

        bool shortperiod = false;

        if (!_bf_edgetrains_addshortperiod(
            wav,
            rightedge,
            leftperiod,
            rightperiod,
            periods[mixed],
            leftcv,
            rightcv,
            deltast,
            &shortperiod
        )) {
            return false;
        }

        if (!_bf_edgetrains_addedgechange(
            wav,
            rightedge,
            leftcv,
            rightcv,
            deltast,
            shortperiod
        )) {
            return false;
        }
    }

    // Table with adders, each receives full data
    // each receives bitmask with other candi-setters setting their bits
    // thus `change` can check for `shortperiod`
    // all these params should be struct and pre-s will make this struct

    return true;
}

/*!
 * \brief  Adds a weak probe near the start of an active span
 *
 * The first observed same-polarity period inside an active span can indicate
 * where the first waveform cycle would have landed if the span boundary did
 * not cut into an attack or noisy edge.  The candidate is emitted only when
 * the extrapolated timestamp lies strictly inside the active span.
 *
 * \param[in,out] wav          The wav
 * \param[in]     spanstart    Active span start in seconds
 * \param[in]     spanend      Active span end in seconds
 * \param[in]     probestart   Candidate timestamp candidate in seconds
 * \param[in]     periodfirst  First same-polarity period inside the span
 * \return        True if processing can continue
 */
static bool _bf_edgetrains_addspanstartprobe(
    struct nsk_wav *wav,
    double          spanstart,
    double          spanend,
    double          probestart,
    double          periodfirst
) {
    if (spanstart >= probestart || probestart >= spanend) {
        return true;
    }

    return nsk_wav_candidate(
        wav,
        (struct nsk_wav_candidate) {
            .method = NSK_WAV_CND_METHOD_SPAN_START_PERIOD_PROBE,
            .kind   = NSK_WAV_CND_KIND_SPAN_START_PROBE,
            .timestamp = spanstart + periodfirst,
            .strength  = nsk_options_program.profile.boundary.edgetrain.strengthspanstartprobe,
            .confidence = nsk_options_program.profile.boundary.edgetrain.confidencespanstartprobe
        }
    );
}

/*!
 * \brief  Adds a weak probe near the end of an active span
 *
 * The last observed same-polarity period inside an active span can indicate
 * where a final waveform cycle would have ended before the signal fell below
 * the active-span threshold.  The candidate is emitted only when the
 * extrapolated timestamp lies strictly inside the active span.
 *
 * \param[in,out] wav         The wav
 * \param[in]     spanstart   Active span start in seconds
 * \param[in]     spanend     Active span end in seconds
 * \param[in]     probeend    Candidate timestamp candidate in seconds
 * \param[in]     periodlast  Last same-polarity period inside the span
 * \return        True if processing can continue
 */
static bool _bf_edgetrains_addspanendprobe(
    struct nsk_wav *wav,
    double          spanstart,
    double          spanend,
    double          probeend,
    double          periodlast
) {
    if (spanstart >= probeend || probeend >= spanend) {
        return true;
    }

    return nsk_wav_candidate(
        wav,
        (struct nsk_wav_candidate) {
            .method = NSK_WAV_CND_METHOD_SPAN_END_PERIOD_PROBE,
            .kind   = NSK_WAV_CND_KIND_SPAN_END_PROBE,
            .timestamp = spanend - periodlast,
            .strength  = nsk_options_program.profile.boundary.edgetrain.strengthspanendprobe,
            .confidence = nsk_options_program.profile.boundary.edgetrain.confidencespanendprobe
        }
    );
}

/*!
 * \brief  Adds period-derived probes near the boundaries of one active span
 *
 * This helper looks at the same-polarity edges that fall strictly inside the
 * supplied active span.  If at least two edges are available, the first period
 * is projected from the span start and the last period is projected backward
 * from the span end.  Both probes are intentionally weak: they preserve edge
 * train evidence near RMS span boundaries without claiming an exact boundary.
 *
 * \param[in,out] wav   The wav
 * \param[in]     span  Active span being processed
 * \param[in]     edge  Same-polarity edge list
 * \return        True if processing can continue
 */
static bool _bf_edgetrains_process_spanprobes(
    struct nsk_wav             *wav,
    const struct nsk_wav_span  *span,
    const struct nsk_wav_edges *edge
) {
    static const size_t _count_spanmin = 2;

    const double spanstart = (double)span->start / wav->format.samplerate;
    const double spanend   = (double)span->end   / wav->format.samplerate;

    size_t edgefirst = 0;
    size_t edgelast  = 0;

    _bf_edgetrains_spanedgerange(
        edge,
        spanstart,
        spanend,
        &edgefirst,
        &edgelast
    );

    const size_t count = edgelast - edgefirst;

    if (count < _count_spanmin) {
        return true;
    }

    const double periodfirst =
        edge->edge[edgefirst + 1] -
        edge->edge[edgefirst];

    const double periodlast =
        edge->edge[edgelast - 1] -
        edge->edge[edgelast - 2];

    const double probestart = spanstart + periodfirst;
    const double probeend   = spanend   - periodlast;

    if (!_bf_edgetrains_addspanstartprobe(
        wav,
        spanstart,
        spanend,
        probestart,
        periodfirst
    )) {
        return false;
    }

    if (!_bf_edgetrains_addspanendprobe(
        wav,
        spanstart,
        spanend,
        probeend,
        periodlast
    )) {
        return false;
    }

    return true;
}

/*!
 * \brief  Finds the boundaries by edge trains
 *
 * Same-polarity edge trains describe full waveform periods without depending
 * on pulse duty.  This stage processes rising and falling trains separately,
 * builds adjacent-edge periods for each train, and compares stable period
 * windows around every possible mixed interval.
 *
 * When the periods before and after such an interval are stable and their
 * pitch distance is large enough, the stage emits period-change candidates.
 * These candidates are evidence that a musical boundary is nearby; later grid
 * and decoder stages decide whether they become accepted note boundaries.
 *
 * \param[in,out]  wav   The wav
 * \return True if all edges were processed successfully
 */
bool nsk_wav_bf_edgetrains(
    struct nsk_wav *wav
) {
    const struct {
        const struct nsk_wav_edges *edge;
    } _table[] = {
        {
            .edge = &wav->edges.rise
        },
        {
            .edge = &wav->edges.fall
        }
    };

    for (size_t ispan = 0; ispan < wav->spans.count; ispan++) {
        const struct nsk_wav_span *span = &wav->spans.span[ispan];
        const double spanstart =
            (double)span->start / wav->format.samplerate;
        const double spanend =
            (double)span->end / wav->format.samplerate;

        for (size_t iedge = 0; iedge < NSK_SIZE(_table); iedge++) {
            size_t edgefirst = 0;
            size_t edgelast  = 0;

            _bf_edgetrains_spanedgerange(
                _table[iedge].edge,
                spanstart,
                spanend,
                &edgefirst,
                &edgelast
            );

            if (!_bf_edgetrains_process_transitioncandidates(
                wav,
                _table[iedge].edge,
                edgefirst,
                edgelast
            )) {
                return false;
            }

            if (!_bf_edgetrains_process_spanprobes(
                wav,
                span,
                _table[iedge].edge
            )) {
                return false;
            }
        }
    }

    return true;
}
