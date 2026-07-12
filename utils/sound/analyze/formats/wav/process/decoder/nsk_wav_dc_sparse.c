#include <float.h>
#include <math.h>
#include <stdlib.h>

#include <nsk_util_meta.h>

#include "nsk_wav_dc_sparse.h"
#include "../../../../arguments/nsk_args_options.h"

/*!
 * \brief  DP back-reference item.
 */
struct _sparse_backref {
    bool found; /*!< Whether this back-reference is valid */

    size_t start;     /*!< Interval start frame */
    size_t corestart; /*!< Weighted core start frame */
    size_t coreend;   /*!< Weighted core end frame */

    int midi; /*!< Chosen MIDI note, or -1 for rest */

    double emission; /*!< Interval state cost */
    double boundary; /*!< Boundary cost */
};

/*!
 * \brief  Appends a raw interval to the interval list.
 *
 * \param[in,out] intervals  Raw interval list
 * \param[in]     interval   Raw interval to append
 * \return True if the interval was appended successfully
 */
static bool _sparse_interval_append(
    struct nsk_wav_dc_intervals *intervals,
    struct nsk_wav_dc_interval interval
) {
    __typeof__(intervals->list) ptr = realloc(
        intervals->list,
        sizeof(*intervals->list) * (intervals->count + 1)
    );

    if (!ptr) {
        nsk_err("Cannot reallocate sparse decoder intervals");
        return false;
    }

    intervals->list = ptr;
    intervals->list[intervals->count++] = interval;

    return true;
}

/*!
 * \brief  Reverses the raw interval list in place.
 *
 * \param[in,out] intervals  Raw interval list
 */
static void _sparse_intervals_reverse(
    struct nsk_wav_dc_intervals *intervals
) {
    for (size_t i = 0; i < intervals->count / 2; i++) {
        const size_t j = intervals->count - i - 1;
        const struct nsk_wav_dc_interval tmp = intervals->list[i];

        intervals->list[i] = intervals->list[j];
        intervals->list[j] = tmp;
    }
}

/*!
 * \brief  Calculates the weighted core bounds for one interval.
 *
 * \param[in]  framestart  Inclusive interval start frame
 * \param[in]  frameend    Exclusive interval end frame
 * \param[out] corestart   Inclusive weighted core start frame
 * \param[out] coreend     Exclusive weighted core end frame
 */
static void _sparse_core_bounds(
    size_t framestart,
    size_t frameend,
    size_t *corestart,
    size_t *coreend
) {
    *corestart = framestart;
    *coreend = frameend;

    const size_t length = frameend - framestart;
    const double fps = nsk_options_program.profile.boundary.grid.fps;
    const double duration = (double)length / fps;
    const double ratiotrim =
        duration *
        NSK_MAX(0.0, nsk_options_program.profile.segments.trimratio);
    const double trim = NSK_MIN(
        NSK_MAX(0.0, nsk_options_program.profile.segments.trim),
        ratiotrim
    );
    const double core = duration - trim * 2.0;

    if (
        trim <= 0.0 ||
        core < nsk_options_program.profile.segments.mincore
    ) {
        return;
    }

    const size_t trimframes = lround(trim * fps);

    if (trimframes == 0 || trimframes * 2 >= length) {
        return;
    }

    *corestart = framestart + trimframes;
    *coreend = frameend - trimframes;
}

/*!
 * \brief  Releases memory owned by a raw interval list.
 *
 * \param[in,out] intervals  Raw interval list to clear
 */
void nsk_wav_dc_intervals_free(
    struct nsk_wav_dc_intervals *intervals
) {
    if (!intervals) {
        return;
    }

    free(intervals->list);
    *intervals = (struct nsk_wav_dc_intervals) {0};
}

/*!
 * \brief  Chooses the best decoder state for one frame interval.
 *
 * \param[in]  framecosts  Per-frame state costs
 * \param[in]  framestart  Inclusive frame start
 * \param[in]  frameend    Exclusive frame end
 * \param[out] interval    Interval result to fill
 * \return True if the interval was scored successfully
 */
bool nsk_wav_dc_sparse_state(
    const struct nsk_wav_dc_framecosts *framecosts,
    size_t framestart,
    size_t frameend,
    struct nsk_wav_dc_interval *interval
) {
    if (
        !framecosts ||
        !interval ||
        framestart >= frameend ||
        frameend > framecosts->framecount
    ) {
        return false;
    }

    *interval = (struct nsk_wav_dc_interval) {
        .framestart = framestart,
        .frameend   = frameend,
        .midi       = -1,
        .emission   = DBL_MAX
    };

    _sparse_core_bounds(
        framestart,
        frameend,
        &interval->corestart,
        &interval->coreend
    );

    for (size_t state = 0; state < framecosts->statecount; state++) {
        const double full = nsk_wav_dc_framecosts_interval(
            framecosts,
            state,
            framestart,
            frameend
        );
        double total = full;

        if (
            interval->corestart != framestart ||
            interval->coreend != frameend
        ) {
            const double core = nsk_wav_dc_framecosts_interval(
                framecosts,
                state,
                interval->corestart,
                interval->coreend
            );
            const double edge = full - core;

            total =
                core +
                edge * nsk_options_program.profile.decoder.edgecostweight;
        }

        const int midi = nsk_wav_dc_framecosts_midi(framecosts, state);
        const size_t length = frameend - framestart;

        if (
            midi >= 0 &&
            length < nsk_options_program.profile.decoder.minnoteframes
        ) {
            const double missing =
                nsk_options_program.profile.decoder.minnoteframes - length;

            total +=
                nsk_options_program.profile.decoder.shortnotepenalty *
                missing *
                missing;
        }

        if (total < interval->emission) {
            interval->midi = midi;
            interval->emission = total;
        }
    }

    return interval->emission < DBL_MAX;
}

/*!
 * \brief  Decodes raw sparse intervals with global dynamic programming.
 *
 * \param[in]  framecosts  Per-frame state costs
 * \param[in]  boundaries  Boundary costs
 * \param[out] intervals   Raw decoded interval list
 * \return True if sparse decoding completed successfully
 */
bool nsk_wav_dc_sparse_decode(
    const struct nsk_wav_dc_framecosts *framecosts,
    const struct nsk_wav_dc_boundaries *boundaries,
    struct nsk_wav_dc_intervals *intervals
) {
    *intervals = (struct nsk_wav_dc_intervals) {0};

    if (!framecosts || !boundaries) {
        return false;
    }

    const size_t maxframe = framecosts->framecount;

    if (maxframe == 0) {
        return true;
    }

    if (boundaries->count < maxframe + 1) {
        nsk_err("Boundary cost table is shorter than frame cost table");
        return false;
    }

    const double infinity = DBL_MAX / 4.0;

    nsk_auto_free double *dp = calloc(maxframe + 1, sizeof(*dp));
    nsk_auto_free struct _sparse_backref *back = calloc(
        maxframe + 1,
        sizeof(*back)
    );

    if (!dp || !back) {
        nsk_err("Cannot allocate sparse decoder DP tables");
        return false;
    }

    for (size_t i = 1; i <= maxframe; i++) {
        dp[i] = infinity;
    }

    const size_t maxlen = NSK_MAX(
        1,
        nsk_options_program.profile.decoder.maxsegmentframes
    );

    for (size_t end = 1; end <= maxframe; end++) {
        const size_t startmin = end > maxlen ? end - maxlen : 0;

        for (size_t start = startmin; start < end; start++) {
            if (dp[start] >= infinity) {
                continue;
            }

            struct nsk_wav_dc_interval interval = {0};

            if (!nsk_wav_dc_sparse_state(
                framecosts,
                start,
                end,
                &interval
            )) {
                return false;
            }

            const double boundary =
                start == 0 ?
                    0.0 :
                    nsk_options_program.profile.decoder.boundarypenalty +
                    nsk_wav_dc_boundaries_cost(boundaries, start);
            const double total =
                dp[start] +
                interval.emission +
                boundary;

            if (total < dp[end]) {
                dp[end] = total;
                back[end] = (struct _sparse_backref) {
                    .found     = true,
                    .start     = start,
                    .corestart = interval.corestart,
                    .coreend   = interval.coreend,
                    .midi      = interval.midi,
                    .emission  = interval.emission,
                    .boundary  = boundary
                };
            }
        }
    }

    size_t cursor = maxframe;

    while (cursor > 0) {
        if (!back[cursor].found) {
            nsk_wav_dc_intervals_free(intervals);
            nsk_err("Sparse decoder backtrace cannot reach frame zero");
            return false;
        }

        const struct _sparse_backref item = back[cursor];

        if (!_sparse_interval_append(
            intervals,
            (struct nsk_wav_dc_interval) {
                .framestart = item.start,
                .frameend   = cursor,
                .corestart  = item.corestart,
                .coreend    = item.coreend,
                .midi       = item.midi,
                .emission   = item.emission,
                .boundary   = item.boundary,
                .total      = dp[cursor]
            }
        )) {
            nsk_wav_dc_intervals_free(intervals);
            return false;
        }

        cursor = item.start;
    }

    _sparse_intervals_reverse(intervals);

    return true;
}
