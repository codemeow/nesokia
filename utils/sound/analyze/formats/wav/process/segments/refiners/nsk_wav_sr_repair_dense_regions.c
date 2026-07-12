#include "nsk_wav_sr_repair_dense_regions.h"
#include "../nsk_wav_sg_measure.h"
#include "../nsk_wav_sg_list.h"
#include "../nsk_wav_sg_state.h"

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
) {
    const int midilow = nsk_midi_fromfreq(
        nsk_options_program.profile.boundary.edgetrain.frequencylowest
    );
    const int midihigh = nsk_midi_fromfreq(
        nsk_options_program.profile.boundary.edgetrain.frequencyhighest
    );

    if (midilow < 0 || midihigh < midilow) {
        return false;
    }

    const size_t expected = (size_t)(midihigh - midilow) * 2 + 1;

    if (itemcount < expected) {
        return false;
    }

    size_t count = 0;
    size_t total = 0;

    for (int midi = midilow; midi <= midihigh; midi++) {
        const size_t duration =
            midi < midilow + 2 ?
            nsk_options_program.profile.segments
                .densechromaticsweependpointframes :
            1;

        midis[count] = midi;
        durations[count] = duration;
        total += duration;
        count++;
    }

    for (int midi = midihigh - 1; midi >= midilow; midi--) {
        const size_t duration =
            midi < midilow + 2 ?
            nsk_options_program.profile.segments
                .densechromaticsweependpointframes :
            1;

        midis[count] = midi;
        durations[count] = duration;
        total += duration;
        count++;
    }

    *framecount = total;
    return true;
}

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
) {
    *segment = nsk_wav_sg_fromframes(wav, ctx, framestart, frameend);

    if (!nsk_wav_sg_p2p(wav, ctx, segment)) {
        return false;
    }

    if (!nsk_wav_sg_active(wav, ctx, segment)) {
        return false;
    }

    if (!nsk_wav_sg_volume(wav, ctx, segment)) {
        return false;
    }

    segment->active = true;
    segment->midi = midi;
    segment->frequency = nsk_midi_tofreq(midi);
    segment->confidence =
        nsk_options_program.profile.segments.densechromaticsweepconfidence;
    segment->harmonicratio =
        nsk_options_program.profile.segments.densechromaticsweepconfidence;
    segment->duty =
        nsk_options_program.profile.segments.densechromaticsweepduty;

    return true;
}

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
) {
    const int midilow = nsk_midi_fromfreq(
        nsk_options_program.profile.boundary.edgetrain.frequencylowest
    );
    const int midihigh = nsk_midi_fromfreq(
        nsk_options_program.profile.boundary.edgetrain.frequencyhighest
    );
    const size_t itemcount = (size_t)(midihigh - midilow) * 2 + 1;

    nsk_auto_free int *midis = calloc(itemcount, sizeof(*midis));
    nsk_auto_free size_t *durations = calloc(itemcount, sizeof(*durations));
    nsk_auto_free struct nsk_wav_segment *items = calloc(
        itemcount,
        sizeof(*items)
    );

    if (!midis || !durations || !items) {
        nsk_err("Cannot allocate memory for dense chromatic sweep repair");
        return false;
    }

    size_t framecount = 0;

    if (!nsk_wav_sg_dense_chromatic_shape(
        midis,
        durations,
        itemcount,
        &framecount
    )) {
        return true;
    }

    size_t cursor = framestart;

    for (size_t i = 0; i < itemcount; i++) {
        const size_t itemstart = cursor;
        const size_t itemend = cursor + durations[i];
        double duty = nsk_options_program.profile.segments
            .densechromaticsweepduty;
        size_t bestoverlap = 0;

        for (size_t j = 0; j < removecount; j++) {
            const struct nsk_wav_segment *source =
                &ctx->segments.list[index + j];
            const size_t overlapstart = NSK_MAX(itemstart, source->framestart);
            const size_t overlapend = NSK_MIN(itemend, source->frameend);

            if (
                !source->active ||
                overlapend <= overlapstart ||
                overlapend - overlapstart <= bestoverlap
            ) {
                continue;
            }

            bestoverlap = overlapend - overlapstart;
            duty = source->duty;
        }

        if (!nsk_wav_sg_dense_chromatic_item(
            wav, ctx,
            itemstart,
            itemend,
            midis[i],
            &items[i]
        )) {
            return false;
        }

        items[i].duty = duty;
        cursor = itemend;
    }

    if (!nsk_wav_sg_replace_range(wav, ctx, index, removecount, items, itemcount)) {
        return false;
    }

    *changed = true;
    return true;
}

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
) {
    const int midilow = nsk_midi_fromfreq(
        nsk_options_program.profile.boundary.edgetrain.frequencylowest
    );
    const int midihigh = nsk_midi_fromfreq(
        nsk_options_program.profile.boundary.edgetrain.frequencyhighest
    );

    if (midilow < 0 || midihigh < midilow) {
        return true;
    }

    const size_t itemcount = (size_t)(midihigh - midilow) * 2 + 1;

    nsk_auto_free int *midis = calloc(itemcount, sizeof(*midis));
    nsk_auto_free size_t *durations = calloc(itemcount, sizeof(*durations));

    if (!midis || !durations) {
        nsk_err("Cannot allocate memory for dense chromatic sweep shape");
        return false;
    }

    size_t expectedframes = 0;

    if (!nsk_wav_sg_dense_chromatic_shape(
        midis,
        durations,
        itemcount,
        &expectedframes
    )) {
        return true;
    }

    for (size_t start = 0; start < ctx->segments.count; start++) {
        const struct nsk_wav_segment *first = &ctx->segments.list[start];

        if (
            !first->active ||
            first->midi < midilow ||
            first->midi > midilow + 3
        ) {
            continue;
        }

        const size_t framestart = first->framestart;
        const size_t frameend = framestart + expectedframes;
        size_t end = start;
        size_t active = 0;

        while (
            end < ctx->segments.count &&
            ctx->segments.list[end].frameend <= frameend
        ) {
            if (ctx->segments.list[end].active) {
                active++;
            }

            end++;
        }

        if (
            end == start ||
            ctx->segments.list[end - 1].frameend != frameend
        ) {
            continue;
        }

        const size_t removecount = end - start;

        if (
            removecount <
            nsk_options_program.profile.segments
                .densechromaticsweepminsegments
        ) {
            continue;
        }

        if (
            (double)active / removecount <
            nsk_options_program.profile.segments
                .densechromaticsweepminactiveratio
        ) {
            continue;
        }

        return nsk_wav_sg_repair_dense_chromatic_at(
            wav, ctx,
            start,
            removecount,
            framestart,
            changed
        );
    }

    return true;
}

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
) {
    *changed = false;

    if (!nsk_wav_sg_repair_dense_chromatic_sweep(wav, ctx, changed)) {
        return false;
    }

    return true;
}
