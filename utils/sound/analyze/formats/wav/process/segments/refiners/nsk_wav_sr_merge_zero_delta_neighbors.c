#include "nsk_wav_sr_merge_zero_delta_neighbors.h"
#include "../nsk_wav_sg_list.h"
#include "../nsk_wav_sg_measure.h"
#include "../nsk_wav_sg_classify.h"
#include "../nsk_wav_sg_template.h"

/*!
 * \brief  Merges isolated one-frame segments into neighbors when boundary delta is zero-like.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[out]  changed  Set to true when the function modifies segment data.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_merge_zero_delta_neighbors(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    bool *changed
) {
    *changed = false;
    nsk_auto_free struct nsk_wav_segment *items = calloc(
        ctx->segments.count,
        sizeof(*items)
    );

    if (!items) {
        nsk_err("Cannot allocate memory for zero-delta segment merge");
        return false;
    }

    size_t itemcount = 0;
    size_t index = 0;

    while (index < ctx->segments.count) {
        const struct nsk_wav_segment current = ctx->segments.list[index];

        if (
            current.frameend - current.framestart != 1 ||
            !current.active
        ) {
            items[itemcount++] = current;
            index++;
            continue;
        }

        if (itemcount > 0 && index + 1 < ctx->segments.count) {
            const struct nsk_wav_segment left = items[itemcount - 1];
            const struct nsk_wav_segment right = ctx->segments.list[index + 1];

            if (
                left.active &&
                right.active &&
                left.frameend - left.framestart == 1 &&
                right.frameend - right.framestart == 1 &&
                abs(left.midi - current.midi) == 1 &&
                abs(current.midi - right.midi) == 1 &&
                left.midi - current.midi == current.midi - right.midi
            ) {
                items[itemcount++] = current;
                index++;
                continue;
            }
        }

        if (itemcount > 0) {
            const struct nsk_wav_segment left = items[itemcount - 1];

            if (
                left.active &&
                left.frameend - left.framestart >=
                nsk_options_program.profile.segments.zerodeltaneighborminframes &&
                nsk_wav_sg_mark_p2p_delta(wav, ctx, current.framestart) <=
                nsk_options_program.profile.segments.zerodeltaneighbormaxp2pdelta
            ) {
                struct nsk_wav_segment merged = nsk_wav_sg_fromframes(
                    wav, ctx,
                    left.framestart,
                    current.frameend
                );

                if (!nsk_wav_sg_recalc(wav, ctx, &merged)) {
                    return false;
                }

                bool ignored = false;

                if (!nsk_wav_sg_segment_template_override(wav, ctx, &merged, &ignored)) {
                    return false;
                }

                if (
                    merged.active &&
                    merged.midi == left.midi &&
                    merged.confidence >=
                    nsk_options_program.profile.segments
                        .zerodeltaneighborminconfidence
                ) {
                    items[itemcount - 1] = merged;
                    *changed = true;
                    index++;
                    continue;
                }
            }
        }

        if (index + 1 < ctx->segments.count) {
            const struct nsk_wav_segment right = ctx->segments.list[index + 1];

            if (
                right.active &&
                right.frameend - right.framestart >=
                nsk_options_program.profile.segments.zerodeltaneighborminframes &&
                nsk_wav_sg_mark_p2p_delta(wav, ctx, current.frameend) <=
                nsk_options_program.profile.segments.zerodeltaneighbormaxp2pdelta
            ) {
                struct nsk_wav_segment merged = nsk_wav_sg_fromframes(
                    wav, ctx,
                    current.framestart,
                    right.frameend
                );

                if (!nsk_wav_sg_recalc(wav, ctx, &merged)) {
                    return false;
                }

                bool ignored = false;

                if (!nsk_wav_sg_segment_template_override(wav, ctx, &merged, &ignored)) {
                    return false;
                }

                if (
                    merged.active &&
                    merged.midi == right.midi &&
                    merged.confidence >=
                    nsk_options_program.profile.segments
                        .zerodeltaneighborminconfidence
                ) {
                    items[itemcount++] = merged;
                    *changed = true;
                    index += 2;
                    continue;
                }
            }
        }

        items[itemcount++] = current;
        index++;
    }

    if (!*changed) {
        return true;
    }

    if (!nsk_wav_sg_replace_range(
        wav, ctx,
        0,
        ctx->segments.count,
        items,
        itemcount
    )) {
        return false;
    }

    return true;
}
