#include "nsk_wav_sr_split_candidate_marks.h"
#include "../nsk_wav_sg_list.h"
#include "../nsk_wav_sg_state.h"
#include "../nsk_wav_sg_classify.h"
#include "../nsk_wav_sg_template.h"

/*!
 * \brief  Evaluates whether a candidate mark should split a segment.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in,out]  segment  Segment being analyzed or updated.
 * \param[in]  frame  Frame index or frame position.
 * \param[in,out]  left  Left segment or left numeric value.
 * \param[in]  right  Right segment or right numeric value.
 * \param[out]  use  Set to true when the candidate split should be applied.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_candidate_split(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    const struct nsk_wav_segment *segment,
    size_t frame,
    struct nsk_wav_segment *left,
    struct nsk_wav_segment *right,
    bool *use
) {
    *use = false;

    if (
        frame <= segment->framestart ||
        frame >= segment->frameend ||
        segment->frameend - segment->framestart >
        nsk_options_program.profile.segments.candidatesplitmaxframes ||
        frame >= ctx->marks.count ||
        ctx->marks.list[frame].count == 0
    ) {
        return true;
    }

    if (
        segment->frameend - segment->framestart <=
        nsk_options_program.profile.segments.framegridsplitmaxframes &&
        !ctx->marks.list[frame].indense
    ) {
        return true;
    }

    if (
        !ctx->marks.list[frame].indense &&
        ctx->marks.list[frame].maxabsdeltast <
        nsk_options_program.profile.segments.candidatesplitmindeltast
    ) {
        return true;
    }

    *left = nsk_wav_sg_fromframes(
        wav, ctx,
        segment->framestart,
        frame
    );
    *right = nsk_wav_sg_fromframes(
        wav, ctx,
        frame,
        segment->frameend
    );

    if (!nsk_wav_sg_recalc(wav, ctx, left)) {
        return false;
    }

    if (!nsk_wav_sg_recalc(wav, ctx, right)) {
        return false;
    }

    bool ignored = false;

    if (!nsk_wav_sg_segment_template_override(wav, ctx, left, &ignored)) {
        return false;
    }

    if (!nsk_wav_sg_segment_template_override(wav, ctx, right, &ignored)) {
        return false;
    }

    *use =
        left->active &&
        right->active &&
        left->midi >= 0 &&
        right->midi >= 0 &&
        left->midi != right->midi &&
        left->confidence >=
        nsk_options_program.profile.segments.edgeconfirmedsplitminstateconfidence &&
        right->confidence >=
        nsk_options_program.profile.segments.edgeconfirmedsplitminstateconfidence;

    return true;
}

/*!
 * \brief  Splits segments at accepted candidate marks when both sides remain valid.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[out]  changed  Set to true when the function modifies segment data.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_split_candidate_marks(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    bool *changed
) {
    *changed = false;

    if (nsk_wav_sg_isdensedirect(wav, ctx)) {
        return true;
    }

    if (
        ctx->segments.count >
        nsk_options_program.profile.segments.candidatesplitmaxsegments
    ) {
        return true;
    }

    nsk_auto_free struct nsk_wav_segment *items = calloc(
        ctx->segments.count + ctx->marks.count,
        sizeof(*items)
    );

    if (!items) {
        nsk_err("Cannot allocate memory for candidate mark segment split");
        return false;
    }

    size_t itemcount = 0;

    for (size_t i = 0; i < ctx->segments.count; i++) {
        struct nsk_wav_segment current = ctx->segments.list[i];

        while (true) {
            bool found = false;
            bool use = false;
            struct nsk_wav_segment left = {0};
            struct nsk_wav_segment right = {0};

            for (
                size_t frame = current.framestart + 1;
                frame < current.frameend;
                frame++
            ) {
                if (!nsk_wav_sg_candidate_split(
                    wav, ctx,
                    &current,
                    frame,
                    &left,
                    &right,
                    &use
                )) {
                    return false;
                }

                if (!use) {
                    continue;
                }

                found = true;
                break;
            }

            if (!found) {
                items[itemcount++] = current;
                break;
            }

            items[itemcount++] = left;
            current = right;
            *changed = true;
        }
    }

    if (!*changed) {
        return true;
    }

    return nsk_wav_sg_replace_range(
        wav, ctx,
        0,
        ctx->segments.count,
        items,
        itemcount
    );
}
