#include <math.h>

#include "nsk_wav_dc_notes.h"
#include "../../nsk_wav_midi.h"
#include "../../../../arguments/nsk_args_options.h"

/*!
 * \brief  Converts one segment into one final decoded event.
 *
 * \param[in] segment  Segment to convert.
 * \return Decoded note/rest event.
 */
static struct nsk_wav_note _dc_notes_from_segment(
    const struct nsk_wav_segment *segment
) {
    if (!segment->active || segment->midi < 0) {
        return (struct nsk_wav_note) {
            .active = false,
            .framestart = segment->framestart,
            .frameend = segment->frameend,
            .samplestart = segment->samplestart,
            .sampleend = segment->sampleend,
            .midi = -1
        };
    }

    return (struct nsk_wav_note) {
        .active = true,
        .framestart = segment->framestart,
        .frameend = segment->frameend,
        .samplestart = segment->samplestart,
        .sampleend = segment->sampleend,
        .midi = segment->midi,
        .frequency =
            segment->frequency > 0.0 ?
            segment->frequency :
            nsk_midi_tofreq(segment->midi),
        .volume = segment->volume,
        .duty = segment->duty
    };
}

/*!
 * \brief  Checks whether two final events belong to the same musical item.
 *
 * \param[in] left   Existing decoded event.
 * \param[in] right  New decoded event.
 * \return True if the events should be merged.
 */
static bool _dc_notes_isequal(
    const struct nsk_wav_note *left,
    const struct nsk_wav_note *right
) {
    if (left->frameend != right->framestart) {
        return false;
    }

    if (!left->active && !right->active) {
        return true;
    }

    return
        left->active &&
        right->active &&
        left->midi == right->midi &&
        fabs(left->volume - right->volume) <=
            nsk_options_program.profile.segments
                .mergevolumeequalitythreshold &&
        fabs(left->duty - right->duty) <=
            nsk_options_program.profile.segments
                .mergedutyequalitythreshold;
}

/*!
 * \brief  Merges a decoded event into an existing final event.
 *
 * \param[in,out] left   Existing decoded event to extend.
 * \param[in]     right  New decoded event to merge into it.
 */
static void _dc_notes_merge(
    struct nsk_wav_note *left,
    const struct nsk_wav_note *right
) {
    const size_t leftframes = left->frameend - left->framestart;
    const size_t rightframes = right->frameend - right->framestart;
    const size_t totalframes = leftframes + rightframes;

    left->frameend = right->frameend;
    left->sampleend = right->sampleend;

    if (!left->active || totalframes == 0) {
        return;
    }

    left->volume =
        (left->volume * leftframes + right->volume * rightframes) /
        totalframes;
    left->duty =
        (left->duty * leftframes + right->duty * rightframes) /
        totalframes;
}

/*!
 * \brief  Appends a final event, merging adjacent equal musical items.
 *
 * \param[in,out] ctx   Processing context receiving decoded events.
 * \param[in]     note  Decoded event to append or merge.
 * \return True if the event was stored successfully.
 */
static bool _dc_notes_append(
    struct nsk_wav_ctx *ctx,
    struct nsk_wav_note note
) {
    if (
        ctx->notes.count > 0 &&
        _dc_notes_isequal(&ctx->notes.list[ctx->notes.count - 1], &note)
    ) {
        _dc_notes_merge(&ctx->notes.list[ctx->notes.count - 1], &note);
        return true;
    }

    return nsk_wav_ctx_note(ctx, note);
}

/*!
 * \brief  Converts the chosen segment hypothesis into final note/rest events.
 *
 * \param[in]      wav  Source WAV data.
 * \param[in,out]  ctx  Processing context receiving decoded events.
 * \return True if decoded events were created successfully.
 */
bool nsk_wav_dc_notes_from_segments(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx *ctx
) {
    (void)wav;

    nsk_wav_ctx_notes_clear(ctx);

    for (size_t i = 0; i < ctx->segments.count; i++) {
        if (!_dc_notes_append(
            ctx,
            _dc_notes_from_segment(&ctx->segments.list[i])
        )) {
            return false;
        }
    }

    return true;
}
