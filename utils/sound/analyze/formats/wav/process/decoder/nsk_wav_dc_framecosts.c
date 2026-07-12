#include <math.h>
#include <stdlib.h>

#include <nsk_util_meta.h>

#include "nsk_wav_dc_framecosts.h"
#include "../segments/nsk_wav_sg_classify.h"
#include "../segments/nsk_wav_sg_list.h"
#include "../segments/nsk_wav_sg_measure.h"
#include "../segments/nsk_wav_sg_period.h"
#include "../../nsk_wav_midi.h"
#include "../../../../arguments/nsk_args_options.h"

/*!
 * \brief  Calculates the state-major matrix offset.
 *
 * \param[in] columns  Number of columns in the matrix
 * \param[in] state    State row index
 * \param[in] frame    Frame column index
 * \return Matrix element offset
 */
static size_t _framecosts_offset(
    size_t columns,
    size_t state,
    size_t frame
) {
    return state * columns + frame;
}

/*!
 * \brief  Calculates the lowest decoder MIDI state.
 *
 * \return Lowest MIDI note used by the decoder
 */
static int _framecosts_midimin(void) {
    return nsk_midi_fromfreq(
        nsk_options_program.profile.boundary.edgetrain.frequencylowest
    );
}

/*!
 * \brief  Calculates the highest decoder MIDI state.
 *
 * \return Highest MIDI note used by the decoder
 */
static int _framecosts_midimax(void) {
    return nsk_midi_fromfreq(
        nsk_options_program.profile.boundary.edgetrain.frequencyhighest
    );
}

/*!
 * \brief  Releases memory owned by a frame cost table.
 *
 * \param[in,out] costs  Frame cost table to clear
 */
void nsk_wav_dc_framecosts_free(struct nsk_wav_dc_framecosts *costs) {
    if (!costs) {
        return;
    }

    free(costs->costs);
    free(costs->prefix);
    free(costs->localactive);
    free(costs->localmidi);
    free(costs->periodmidi);
    free(costs->periodscore);

    *costs = (struct nsk_wav_dc_framecosts) {0};
}

/*!
 * \brief  Returns the MIDI note represented by a decoder state.
 *
 * \param[in] costs  Frame cost table
 * \param[in] state  Decoder state index
 * \return MIDI note for note states, or -1 for rest/invalid states
 */
int nsk_wav_dc_framecosts_midi(
    const struct nsk_wav_dc_framecosts *costs,
    size_t state
) {
    if (!costs || state == 0 || state >= costs->statecount) {
        return -1;
    }

    return costs->midimin + (int)state - 1;
}

/*!
 * \brief  Returns one frame cost value.
 *
 * \param[in] costs  Frame cost table
 * \param[in] state  Decoder state index
 * \param[in] frame  Frame index
 * \return Cost value, or 0 when the coordinates are outside the table
 */
double nsk_wav_dc_framecosts_cost(
    const struct nsk_wav_dc_framecosts *costs,
    size_t state,
    size_t frame
) {
    if (
        !costs ||
        state >= costs->statecount ||
        frame >= costs->framecount
    ) {
        return 0.0;
    }

    return costs->costs[
        _framecosts_offset(costs->framecount, state, frame)
    ];
}

/*!
 * \brief  Returns an interval cost from prefix sums.
 *
 * \param[in] costs       Frame cost table
 * \param[in] state       Decoder state index
 * \param[in] framestart  Inclusive frame start
 * \param[in] frameend    Exclusive frame end
 * \return Cost value, or 0 when the coordinates are outside the table
 */
double nsk_wav_dc_framecosts_interval(
    const struct nsk_wav_dc_framecosts *costs,
    size_t state,
    size_t framestart,
    size_t frameend
) {
    if (
        !costs ||
        state >= costs->statecount ||
        framestart > frameend ||
        frameend > costs->framecount
    ) {
        return 0.0;
    }

    const size_t columns = costs->framecount + 1;
    const size_t start = _framecosts_offset(columns, state, framestart);
    const size_t end = _framecosts_offset(columns, state, frameend);

    return costs->prefix[end] - costs->prefix[start];
}

/*!
 * \brief  Scores one frame against one decoder state.
 *
 * \param[in] segment      One-frame local classification result
 * \param[in] period       One-frame period evidence
 * \param[in] candidatemidi  Decoder state MIDI note, or -1 for rest
 * \return Per-frame state cost
 */
static double _framecosts_score(
    const struct nsk_wav_segment *segment,
    const struct _segment_pitch *period,
    int candidatemidi
) {
    const bool active = segment->active;
    double cost = 0.0;

    if (candidatemidi < 0) {
        cost = active ?
            nsk_options_program.profile.decoder.restnotemismatchcost :
            0.0;
    } else if (!active) {
        cost = nsk_options_program.profile.decoder.restnotemismatchcost;
    } else if (segment->midi < 0) {
        cost = nsk_options_program.profile.decoder.restnotemismatchcost;
    } else {
        cost = NSK_MIN(
            nsk_options_program.profile.decoder.maxmididistancecost,
            fabs((double)candidatemidi - segment->midi) *
            nsk_options_program.profile.decoder.mididistanceweight
        );
    }

    if (
        candidatemidi >= 0 &&
        period->found &&
        period->confidence >= nsk_options_program.profile.decoder.periodminscore &&
        active
    ) {
        const double periodcost =
            NSK_MAX(0.0, 1.0 - period->confidence) *
            nsk_options_program.profile.decoder.periodresidualweight +
            fabs((double)candidatemidi - period->midi) *
            nsk_options_program.profile.decoder.periodmididistanceweight;

        cost = NSK_MIN(cost, periodcost);
    }

    return cost;
}

/*!
 * \brief  Builds decoder per-frame state costs and prefix sums.
 *
 * \param[in]      wav    Source WAV data
 * \param[in,out]  ctx    Processing context
 * \param[out]     costs  Frame cost table to fill
 * \return True if the table was built successfully
 */
bool nsk_wav_dc_framecosts_build(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    struct nsk_wav_dc_framecosts *costs
) {
    *costs = (struct nsk_wav_dc_framecosts) {0};

    if (ctx->marks.count == 0) {
        return true;
    }

    if (ctx->segments.silence <= 0.0) {
        if (!nsk_wav_sg_calcsilence(wav, ctx)) {
            return false;
        }
    }

    costs->framecount = ctx->marks.count;
    costs->midimin = _framecosts_midimin();
    costs->midimax = _framecosts_midimax();

    if (costs->midimax < costs->midimin) {
        nsk_err("Invalid decoder MIDI range");
        return false;
    }

    costs->statecount = (size_t)(costs->midimax - costs->midimin + 1) + 1;

    costs->costs = calloc(
        costs->statecount * costs->framecount,
        sizeof(*costs->costs)
    );
    costs->prefix = calloc(
        costs->statecount * (costs->framecount + 1),
        sizeof(*costs->prefix)
    );
    costs->localactive = calloc(
        costs->framecount,
        sizeof(*costs->localactive)
    );
    costs->localmidi = calloc(
        costs->framecount,
        sizeof(*costs->localmidi)
    );
    costs->periodmidi = calloc(
        costs->framecount,
        sizeof(*costs->periodmidi)
    );
    costs->periodscore = calloc(
        costs->framecount,
        sizeof(*costs->periodscore)
    );

    if (
        !costs->costs ||
        !costs->prefix ||
        !costs->localactive ||
        !costs->localmidi ||
        !costs->periodmidi ||
        !costs->periodscore
    ) {
        nsk_wav_dc_framecosts_free(costs);
        nsk_err("Cannot allocate decoder frame cost table");
        return false;
    }

    for (size_t frame = 0; frame < costs->framecount; frame++) {
        costs->localmidi[frame] = -1;
        costs->periodmidi[frame] = -1;
    }

    for (size_t frame = 0; frame < costs->framecount; frame++) {
        struct nsk_wav_segment segment =
            nsk_wav_sg_fromframes(wav, ctx, frame, frame + 1);
        struct _segment_pitch period = {0};

        if (!nsk_wav_sg_recalc(wav, ctx, &segment)) {
            nsk_wav_dc_framecosts_free(costs);
            return false;
        }

        if (!nsk_wav_sg_period_note(
            wav, ctx,
            segment.samplestart,
            segment.sampleend,
            &period
        )) {
            nsk_wav_dc_framecosts_free(costs);
            return false;
        }

        costs->localactive[frame] = segment.active && segment.midi >= 0;
        costs->localmidi[frame] =
            costs->localactive[frame] ? segment.midi : -1;
        costs->periodmidi[frame] = period.found ? period.midi : -1;
        costs->periodscore[frame] = period.found ? period.confidence : 0.0;

        for (size_t state = 0; state < costs->statecount; state++) {
            const int midi = nsk_wav_dc_framecosts_midi(costs, state);
            costs->costs[
                _framecosts_offset(costs->framecount, state, frame)
            ] = _framecosts_score(&segment, &period, midi);
        }
    }

    for (size_t state = 0; state < costs->statecount; state++) {
        for (size_t frame = 0; frame < costs->framecount; frame++) {
            const size_t src =
                _framecosts_offset(costs->framecount, state, frame);
            const size_t prev =
                _framecosts_offset(costs->framecount + 1, state, frame);
            const size_t next =
                _framecosts_offset(costs->framecount + 1, state, frame + 1);

            costs->prefix[next] = costs->prefix[prev] + costs->costs[src];
        }
    }

    return true;
}
