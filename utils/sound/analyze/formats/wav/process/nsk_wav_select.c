#include <math.h>
#include <float.h>

#include <nsk_util_meta.h>

#include "nsk_wav_select.h"
#include "../../../arguments/nsk_args_options.h"

/*!
 * \brief  Prints the selected marks count
 *
 * \param[in] wav  The WAV data
 */
static void _select_log(
    const struct nsk_wav      *wav
) {
    size_t accepted = 0;
    for (size_t i = 0; i < wav->marks.count; i++) {
        if (wav->marks.list[i].accepted) {
            accepted++;
        }
    }

    nsk_inf("    - (%zu accepted marks)\n", accepted);
}

/*!
 * \brief  Checks if the candidate method is a transition probe
 *
 * \param[in] method  The candidate method
 * \return True if the method is a transition probe method
 */
static bool _filter_transition(
    enum nsk_wav_cnd_method method
) {
    return
        method == NSK_WAV_CND_METHOD_TRANSITION_PROBES;
}

/*!
 * \brief  Checks if the candidate method is a mixed-period solver
 *
 * \param[in] method  The candidate method
 * \return True if the method is a mixed-period solver method
 */
static bool _filter_mixed(
    enum nsk_wav_cnd_method method
) {
    return
        method == NSK_WAV_CND_METHOD_MIXED_PERIOD_SOLVE;
}

/*!
 * \brief  Checks if the candidate method is period-transition evidence
 *
 * \param[in] method  The candidate method
 * \return True if the method is period-transition evidence
 */
static bool _filter_period(
    enum nsk_wav_cnd_method method
) {
    return
        method == NSK_WAV_CND_METHOD_MIXED_PERIOD_SOLVE ||
        method == NSK_WAV_CND_METHOD_SHORT_PERIOD_EDGE  ||
        method == NSK_WAV_CND_METHOD_EDGE_CHANGE;
}

/*!
 * \brief  Finds the smallest grid error for candidates matching a filter
 *
 * The returned value is measured in seconds.  If no candidate matches the
 * filter, #DBL_MAX is returned so threshold comparisons fail naturally.
 *
 * \param[in] mark    The quantized mark
 * \param[in] filter  Candidate method predicate
 * \return Minimum absolute distance from the mark frame time
 */
static double _mark_griderror(
    const struct nsk_wav_mark *mark,
    bool (*filter) (
        enum nsk_wav_cnd_method method
    )
) {
    double griderrormin = DBL_MAX;

    const double marktime =
        (double)mark->frame /
        nsk_options_program.profile.boundary.grid.fps;

    for (size_t i = 0; i < mark->count; i++) {
        const struct nsk_wav_candidate *candidate = mark->candidates[i];

        if (!filter(candidate->method)) {
            continue;
        }

        const double griderror = fabs(candidate->timestamp - marktime);
        griderrormin = NSK_MIN(griderrormin, griderror);
    }

    return griderrormin;
}

/*!
 * \brief  Selects a mark by grid-close energy evidence
 *
 * Energy onset, offset, and grid-energy-rise candidates are accepted only when
 * their physical timestamp is close enough to the mark frame.
 *
 * \param[in] mark  The quantized mark
 * \return True if the mark has enough energy evidence
 */
static bool _selector_byenergy(
    const struct nsk_wav_mark *mark
) {
    if (
        nsk_options_program.profile.select.energycandidatesthreshold == 0
    ) {
        return false;
    }

    const double marktime =
        (double)mark->frame /
        nsk_options_program.profile.boundary.grid.fps;
    size_t energycandidates = 0;

    for (size_t i = 0; i < mark->count; i++) {
        const struct nsk_wav_candidate *candidate = mark->candidates[i];

        if (
            candidate->method != NSK_WAV_CND_METHOD_ENERGY_ONSET &&
            candidate->method != NSK_WAV_CND_METHOD_ENERGY_OFFSET &&
            candidate->method != NSK_WAV_CND_METHOD_GRID_ENERGY_RISE
        ) {
            continue;
        }

        if (
            fabs(candidate->timestamp - marktime) <=
            nsk_options_program.profile.select.energygriderror
        ) {
            energycandidates++;
            if (
                energycandidates >=
                nsk_options_program.profile.select.energycandidatesthreshold
            ) {
                return true;
            }
        }
    }

    return false;
}

/*!
 * \brief  Selects a mark by one strong period solver candidate
 *
 * Strong period evidence is limited to mixed-period solve and short-period edge
 * candidates.
 *
 * \param[in] mark  The quantized mark
 * \return True if the mark has a strong real period candidate
 */
static bool _selector_bystrength(
    const struct nsk_wav_mark *mark
) {
    if (
        nsk_options_program.profile.select.strongcandidatesthreshold == 0
    ) {
        return false;
    }

    size_t strongs =
        mark->types[NSK_WAV_CND_METHOD_MIXED_PERIOD_SOLVE   ] +
        mark->types[NSK_WAV_CND_METHOD_SHORT_PERIOD_EDGE    ];

    return
        strongs >= nsk_options_program.profile.select.strongcandidatesthreshold &&
        mark->maxstrength >= nsk_options_program.profile.select.strengththreshold;
}

/*!
 * \brief  Selects a mark by multiple real period evidences
 *
 * This accepts moderate period evidence when at least two real candidates agree
 * that a meaningful pitch transition happened near the frame.
 *
 * \param[in] mark  The quantized mark
 * \return True if the mark has enough real period evidence
 */
static bool _selector_bytrusted(
    const struct nsk_wav_mark *mark
) {
    if (
        nsk_options_program.profile.select.periodcandidatesthreshold == 0
    ) {
        return false;
    }

    size_t periods =
        mark->types[NSK_WAV_CND_METHOD_MIXED_PERIOD_SOLVE   ] +
        mark->types[NSK_WAV_CND_METHOD_SHORT_PERIOD_EDGE    ] +
        mark->types[NSK_WAV_CND_METHOD_EDGE_CHANGE          ];

    return
        periods  >= nsk_options_program.profile.select.periodcandidatesthreshold &&
        mark->maxabsdeltast >= nsk_options_program.profile.select.deltastthreshold;
}

/*!
 * \brief  Selects a mark by real period evidence plus transition probes
 *
 * Transition probes alone are not enough here; at least one real period
 * candidate must also be present.
 *
 * \param[in] mark  The quantized mark
 * \return True if the mark passes transition rescue thresholds
 */
static bool _selector_byperiodtransition(
    const struct nsk_wav_mark *mark
) {
    if (
        nsk_options_program.profile.select.transitionperiodcandidatesthreshold == 0 ||
        nsk_options_program.profile.select.transitionprobecandidatesthreshold  == 0
    ) {
        return false;
    }

    const size_t periods =
        mark->types[NSK_WAV_CND_METHOD_MIXED_PERIOD_SOLVE   ] +
        mark->types[NSK_WAV_CND_METHOD_SHORT_PERIOD_EDGE    ] +
        mark->types[NSK_WAV_CND_METHOD_EDGE_CHANGE          ];

    return
        periods >=
            nsk_options_program.profile.select.transitionperiodcandidatesthreshold &&
        mark->types[NSK_WAV_CND_METHOD_TRANSITION_PROBES] >=
            nsk_options_program.profile.select.transitionprobecandidatesthreshold &&
        mark->maxstrength >=
            nsk_options_program.profile.select.transitionstrengththreshold &&
        mark->maxabsdeltast >=
            nsk_options_program.profile.select.transitiondeltastthreshold;
}

/*!
 * \brief  Selects a mark by one high-quality mixed-period candidate
 *
 * This is the sparse single-mixed rescue path.  It uses only mixed-period
 * solver candidates and also requires the best matching candidate to be close
 * enough to the grid frame.
 *
 * \param[in] mark  The quantized mark
 * \return True if the mark passes single-mixed rescue thresholds
 */
static bool _selector_bysinglemixedperiod(
    const struct nsk_wav_mark *mark
) {
    if (
        nsk_options_program.profile.select.singlemixedcandidatesthreshold == 0
    ) {
        return false;
    }

    double strengthmax  = 0.0;
    double deltastmax   = 0.0;
    const double griderrormin = _mark_griderror(mark, _filter_mixed);

    for (size_t i = 0; i < mark->count; i++) {
        const struct nsk_wav_candidate *candidate = mark->candidates[i];

        if (candidate->method != NSK_WAV_CND_METHOD_MIXED_PERIOD_SOLVE) {
            continue;
        }

        strengthmax  = NSK_MAX(strengthmax, candidate->strength);
        deltastmax   = NSK_MAX(deltastmax,  fabs(candidate->deltast));
    }

    return
        mark->types[NSK_WAV_CND_METHOD_MIXED_PERIOD_SOLVE] >=
            nsk_options_program.profile.select.singlemixedcandidatesthreshold &&
        strengthmax >=
            nsk_options_program.profile.select.singlemixedstrengththreshold &&
        deltastmax  >=
            nsk_options_program.profile.select.singlemixeddeltastthreshold &&
        griderrormin <=
            nsk_options_program.profile.select.singlemixedgriderror;
}

/*!
 * \brief  Checks whether a mark is inside a locally dense accepted region
 *
 * The dense decision is based on marks accepted by the primary selectors before
 * dense rescue runs.  Frame 0 is ignored because note boundaries are internal
 * grid frames.
 *
 * \param[in] wav    The WAV data
 * \param[in] index  Mark index to classify
 * \return True if the mark is in a locally dense region
 */
static bool _mark_isdense(
    const struct nsk_wav *wav,
    size_t                index
) {
    const size_t radius = nsk_options_program.profile.select.denseradius;

    if (index == 0 || wav->marks.count <= 1) {
        return false;
    }

    const size_t imin = index > radius ? index - radius : 1;
    const size_t imax = NSK_MIN(
        wav->marks.count - 1,
        index + radius
    );
    const size_t icount = imax - imin + 1;

    size_t accepted = 0;
    for (size_t i = imin; i <= imax; i++) {
        if (wav->marks.list[i].accepted) {
            accepted++;
        }
    }

    return
        (double)accepted / icount >= nsk_options_program.profile.select.denseratiothreshold &&
        accepted >= nsk_options_program.profile.select.densecountthreshold;
}

/*!
 * \brief  Precomputes dense-region flags for all marks
 *
 * This pass freezes local density before secondary dense selectors can accept
 * additional marks.  That prevents dense rescue from reinforcing itself during
 * a single traversal.
 *
 * \param[in,out] wav  The WAV data
 */
static void _marks_calcdense(
    struct nsk_wav *wav
)  {
    for (size_t i = 0; i < wav->marks.count; i++) {
        wav->marks.list[i].indense = _mark_isdense(wav, i);
    }
}

/*!
 * \brief  Selects dense marks supported only by transition probes
 *
 * In locally dense material, a slot with multiple transition probes can be
 * rescued when no edge probe or real period candidate is present and the probe
 * evidence is close enough to the frame grid.
 *
 * \param[in] mark  The quantized mark
 * \return True if the mark passes dense probe rescue thresholds
 */
static bool _selector_byprobes(
    const struct nsk_wav_mark *mark
) {
    if (!mark->indense) {
        return false;
    }

    return
        mark->types[NSK_WAV_CND_METHOD_TRANSITION_PROBES] >=
            nsk_options_program.profile.select.densetransitionsthreshold &&
        mark->types[NSK_WAV_CND_METHOD_EDGE_PROBE] == 0 &&
        mark->types[NSK_WAV_CND_METHOD_MIXED_PERIOD_SOLVE] == 0 &&
        mark->types[NSK_WAV_CND_METHOD_SHORT_PERIOD_EDGE] == 0 &&
        mark->types[NSK_WAV_CND_METHOD_EDGE_CHANGE] == 0 &&
        mark->maxstrength >=
            nsk_options_program.profile.select.transitionstrengththreshold &&
        mark->maxabsdeltast >=
            nsk_options_program.profile.select.transitiondeltastthreshold &&
        _mark_griderror(mark, _filter_transition) <=
            nsk_options_program.profile.select.energygriderror;

}

/*!
 * \brief  Selects dense marks with relaxed period-transition thresholds
 *
 * Dense one-frame material can measure a real semitone transition below the
 * sparse selector thresholds.  This rescue accepts multiple real period
 * candidates using dense-specific strength and semitone thresholds.
 *
 * \param[in] mark  The quantized mark
 * \return True if the mark passes dense period rescue thresholds
 */
static bool _selector_bydenseperiod(
    const struct nsk_wav_mark *mark
) {
    if (!mark->indense) {
        return false;
    }

    const size_t periods =
        mark->types[NSK_WAV_CND_METHOD_MIXED_PERIOD_SOLVE   ] +
        mark->types[NSK_WAV_CND_METHOD_SHORT_PERIOD_EDGE    ] +
        mark->types[NSK_WAV_CND_METHOD_EDGE_CHANGE          ];

    return
        periods >=
            nsk_options_program.profile.select.denseperiodsthreshold &&
        mark->maxabsdeltast >=
            nsk_options_program.profile.select.densedeltastthreshold &&
        mark->maxstrength >=
            nsk_options_program.profile.select.densestrengththreshold &&
        _mark_griderror(mark, _filter_period) <=
            nsk_options_program.profile.quantization.windowerror;
}

/*!
 * \brief  Accepts the time marks that pass the selection
 *
 * \param[in,out]  wav   The wav
 * \return True if selection completed successfully
 */
bool nsk_wav_select(
    struct nsk_wav      *wav
) {
    static const struct {
        bool (*selector)(
            const struct nsk_wav_mark *mark
        );
    } _selectors_primary[] = {
        {
            .selector = _selector_byenergy
        },
        {
            .selector = _selector_bystrength
        },
        {
            .selector = _selector_bytrusted
        },
        {
            .selector = _selector_byperiodtransition
        },
        {
            .selector = _selector_bysinglemixedperiod
        }
    };

    static const struct {
        bool (*selector)(
            const struct nsk_wav_mark *mark
        );
    } _selectors_secondary[] = {
        {
            .selector = _selector_byprobes
        },
        {
            .selector = _selector_bydenseperiod
        }
    };

    for (size_t marki = 0; marki < wav->marks.count; marki++) {
        struct nsk_wav_mark *mark = &wav->marks.list[marki];

        if (mark->count == 0) {
            continue;
        }

        for (size_t i = 0; i < NSK_SIZE(_selectors_primary); i++) {
            if (_selectors_primary[i].selector(mark)) {
                mark->accepted = true;
                break;
            }
        }
    }

    _marks_calcdense(wav);

    for (size_t marki = 0; marki < wav->marks.count; marki++) {
        struct nsk_wav_mark *mark = &wav->marks.list[marki];

        if (
            mark->count == 0 ||
            mark->accepted
        ) {
            continue;
        }

        for (size_t i = 0; i < NSK_SIZE(_selectors_secondary); i++) {
            if (_selectors_secondary[i].selector(mark)) {
                mark->accepted = true;
                break;
            }
        }
    }

    _select_log(wav);

    return true;
}
