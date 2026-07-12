#include <stdlib.h>

#include <nsk_util_meta.h>

#include "nsk_wav_dc_snap.h"
#include "../../../../arguments/nsk_args_options.h"

/*!
 * Number of semitones per octave.
 */
static const size_t semitones_peroctave = 12;

/*!
 * \brief  Snap action flags used between snap passes.
 */
enum _snap_action {
    _SNAP_ACTION_NONE,
    _SNAP_ACTION_TRANSITION_SMEAR
};

/*!
 * \brief  Returns interval length in frames.
 *
 * \param[in] interval  Raw interval
 * \return Frame length
 */
static size_t _snap_length(const struct nsk_wav_dc_interval *interval) {
    return interval->frameend - interval->framestart;
}

/*!
 * \brief  Checks whether an interval is a note.
 *
 * \param[in] interval  Raw interval
 * \return True if the interval is a note
 */
static bool _snap_isnote(const struct nsk_wav_dc_interval *interval) {
    return interval->midi >= 0;
}

/*!
 * \brief  Adjusts an interval end and keeps core bounds valid.
 *
 * \param[in,out] interval  Interval to adjust
 * \param[in]     frameend  New exclusive end frame
 */
static void _snap_set_end(
    struct nsk_wav_dc_interval *interval,
    size_t frameend
) {
    interval->frameend = frameend;
    interval->coreend = NSK_MIN(interval->coreend, frameend);
}

/*!
 * \brief  Adjusts an interval start and keeps core bounds valid.
 *
 * \param[in,out] interval    Interval to adjust
 * \param[in]     framestart  New inclusive start frame
 */
static void _snap_set_start(
    struct nsk_wav_dc_interval *interval,
    size_t framestart
) {
    interval->framestart = framestart;
    interval->corestart = NSK_MAX(interval->corestart, framestart);
}

/*!
 * \brief  Applies chromatic transition smear.
 *
 * \param[in]      framecosts  Per-frame decoder evidence
 * \param[in,out]  intervals   Raw sparse intervals to adjust
 * \param[in,out]  actions     Snap action flags by interval
 */
static void _snap_transition_smear(
    const struct nsk_wav_dc_framecosts *framecosts,
    struct nsk_wav_dc_intervals *intervals,
    enum _snap_action *actions
) {
    if (!nsk_options_program.profile.decoder.enabletransitionsmear) {
        return;
    }

    for (size_t i = 0; i + 1 < intervals->count; i++) {
        struct nsk_wav_dc_interval *current = &intervals->list[i];
        struct nsk_wav_dc_interval *next = &intervals->list[i + 1];

        if (
            !_snap_isnote(current) ||
            !_snap_isnote(next) ||
            current->frameend != next->framestart ||
            _snap_length(current) >
                nsk_options_program.profile.decoder.smearmaxshortframes ||
            _snap_length(next) <
                nsk_options_program.profile.decoder.smearminlongframes ||
            (
                current->midi != next->midi - 1 &&
                current->midi != next->midi - 2
            )
        ) {
            continue;
        }

        const int lower = next->midi - 2;
        const int frame_note =
            current->framestart < framecosts->framecount ?
            framecosts->localmidi[current->framestart] :
            -1;

        if (frame_note != lower && current->midi != lower) {
            continue;
        }

        const size_t newend = NSK_MIN(
            next->frameend,
            current->framestart +
                nsk_options_program.profile.decoder.smeartargetframes
        );

        if (
            newend <= current->frameend ||
            next->frameend - newend <
                nsk_options_program.profile.decoder.minnoteframes
        ) {
            continue;
        }

        current->midi = lower;
        _snap_set_end(current, newend);
        _snap_set_start(next, newend);
        actions[i] = _SNAP_ACTION_TRANSITION_SMEAR;
    }
}

/*!
 * \brief  Applies five-frame note boundary adjustment.
 *
 * \param[in,out] intervals  Raw sparse intervals to adjust
 */
static void _snap_six_frame(struct nsk_wav_dc_intervals *intervals) {
    if (!nsk_options_program.profile.decoder.enablesixframesnap) {
        return;
    }

    for (size_t i = 0; i < intervals->count; i++) {
        struct nsk_wav_dc_interval *current = &intervals->list[i];

        if (!_snap_isnote(current) || _snap_length(current) != 5) {
            continue;
        }

        bool shifted = false;

        if (i > 0) {
            struct nsk_wav_dc_interval *previous = &intervals->list[i - 1];

            if (
                _snap_isnote(previous) &&
                previous->frameend == current->framestart &&
                _snap_length(previous) == 7 &&
                current->frameend - (current->framestart - 1) == 6 &&
                current->framestart - 1 > previous->framestart
            ) {
                const size_t newboundary = current->framestart - 1;

                previous->frameend = newboundary;
                previous->coreend = NSK_MIN(previous->coreend, newboundary);
                current->framestart = newboundary;
                current->corestart = NSK_MAX(
                    current->corestart > 0 ? current->corestart - 1 : 0,
                    newboundary
                );
                shifted = true;
            }
        }

        if (shifted || i + 1 >= intervals->count) {
            continue;
        }

        struct nsk_wav_dc_interval *next = &intervals->list[i + 1];

        if (
            next->framestart == current->frameend &&
            _snap_length(next) >
                nsk_options_program.profile.decoder.minnoteframes &&
            current->frameend + 1 <= next->frameend
        ) {
            const size_t newboundary = current->frameend + 1;

            current->frameend = newboundary;
            current->coreend = NSK_MIN(
                current->coreend + 1,
                newboundary
            );
            _snap_set_start(next, newboundary);
        }
    }
}

/*!
 * \brief  Applies residual snap patterns.
 *
 * \param[in,out] intervals  Raw sparse intervals to adjust
 * \param[in]     actions    Snap action flags by interval
 */
static void _snap_residual(
    struct nsk_wav_dc_intervals *intervals,
    const enum _snap_action *actions
) {
    if (!nsk_options_program.profile.decoder.enableresidualsnap) {
        return;
    }

    for (size_t i = 1; i + 1 < intervals->count; i++) {
        struct nsk_wav_dc_interval *previous = &intervals->list[i - 1];
        struct nsk_wav_dc_interval *current = &intervals->list[i];
        struct nsk_wav_dc_interval *next = &intervals->list[i + 1];

        if (
            actions[i] == _SNAP_ACTION_TRANSITION_SMEAR &&
            _snap_isnote(previous) &&
            _snap_isnote(current) &&
            _snap_isnote(next) &&
            _snap_length(previous) == 23 &&
            _snap_length(current) ==
                nsk_options_program.profile.decoder.smeartargetframes &&
            _snap_length(next) >=
                nsk_options_program.profile.decoder.smearminlongframes
        ) {
            previous->frameend++;
            previous->coreend = NSK_MIN(
                previous->coreend + 1,
                previous->frameend
            );
            current->framestart++;
            current->corestart = NSK_MAX(
                current->corestart + 1,
                current->framestart
            );
            current->frameend++;
            current->coreend = NSK_MIN(
                current->coreend + 1,
                current->frameend
            );
            next->framestart++;
            next->corestart = NSK_MAX(
                next->corestart + 1,
                next->framestart
            );
        }
    }

    for (size_t i = 1; i + 2 < intervals->count; i++) {
        struct nsk_wav_dc_interval *previous = &intervals->list[i - 1];
        struct nsk_wav_dc_interval *current = &intervals->list[i];
        struct nsk_wav_dc_interval *next = &intervals->list[i + 1];
        const struct nsk_wav_dc_interval *after = &intervals->list[i + 2];

        if (
            !_snap_isnote(previous) &&
            _snap_isnote(current) &&
            _snap_isnote(next) &&
            !_snap_isnote(after) &&
            _snap_length(previous) == 2 &&
            _snap_length(current) == 6 &&
            _snap_length(next) == 4 &&
            _snap_length(after) >= 2
        ) {
            previous->frameend++;
            previous->coreend = NSK_MIN(
                previous->coreend + 1,
                previous->frameend
            );
            current->framestart++;
            current->corestart = NSK_MAX(
                current->corestart + 1,
                current->framestart
            );
            current->frameend++;
            current->coreend = NSK_MIN(
                current->coreend + 1,
                current->frameend
            );
            next->framestart++;
            next->corestart = NSK_MAX(
                next->corestart + 1,
                next->framestart
            );
        }
    }
}

/*!
 * \brief  Applies octave boundary adjustment from period evidence.
 *
 * \param[in]      framecosts  Per-frame decoder evidence
 * \param[in,out]  intervals   Raw sparse intervals to adjust
 */
static void _snap_octave_boundary(
    const struct nsk_wav_dc_framecosts *framecosts,
    struct nsk_wav_dc_intervals *intervals
) {
    if (!nsk_options_program.profile.decoder.enableoctaveboundarysnap) {
        return;
    }

    for (size_t i = 0; i + 1 < intervals->count; i++) {
        struct nsk_wav_dc_interval *current = &intervals->list[i];
        struct nsk_wav_dc_interval *next = &intervals->list[i + 1];

        if (
            !_snap_isnote(current) ||
            !_snap_isnote(next) ||
            current->frameend != next->framestart ||
            abs(current->midi - next->midi) != (int)semitones_peroctave ||
            _snap_length(current) <=
                nsk_options_program.profile.decoder.minnoteframes ||
            _snap_length(next) <=
                nsk_options_program.profile.decoder.minnoteframes
        ) {
            continue;
        }

        const size_t edgeframe = current->frameend - 1;

        if (
            edgeframe >= framecosts->framecount ||
            framecosts->periodmidi[edgeframe] != next->midi ||
            framecosts->periodscore[edgeframe] <
                nsk_options_program.profile.decoder
                    .octaveboundaryperiodminscore
        ) {
            continue;
        }

        current->frameend--;
        current->coreend = NSK_MIN(current->coreend, current->frameend);
        next->framestart--;
        next->corestart = NSK_MAX(
            next->corestart > 0 ? next->corestart - 1 : 0,
            next->framestart
        );
    }
}

/*!
 * \brief  Applies default-enabled post-DP boundary adjustments.
 *
 * \param[in]      framecosts  Per-frame decoder evidence
 * \param[in,out]  intervals   Raw sparse intervals to adjust
 * \return True if snap passes completed successfully
 */
bool nsk_wav_dc_snap_apply(
    const struct nsk_wav_dc_framecosts *framecosts,
    struct nsk_wav_dc_intervals *intervals
) {
    if (!framecosts || !intervals) {
        return false;
    }

    nsk_auto_free enum _snap_action *actions = calloc(
        intervals->count,
        sizeof(*actions)
    );

    if (!actions && intervals->count > 0) {
        nsk_err("Cannot allocate decoder snap action flags");
        return false;
    }

    _snap_transition_smear(framecosts, intervals, actions);
    _snap_six_frame(intervals);
    _snap_residual(intervals, actions);
    _snap_octave_boundary(framecosts, intervals);

    return true;
}
