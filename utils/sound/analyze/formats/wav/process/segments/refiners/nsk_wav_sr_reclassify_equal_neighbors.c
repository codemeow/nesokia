#include "nsk_wav_sr_reclassify_equal_neighbors.h"
#include "../nsk_wav_sg_list.h"
#include "../nsk_wav_sg_classify.h"
#include "../nsk_wav_sg_template.h"

/*!
 * \brief  Reclassifies a run of equal neighboring segments as one analysis window.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[out]  changed  Set to true when the function modifies segment data.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_reclassify_equal_neighbors(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    bool *changed
) {
    *changed = false;

    if (ctx->segments.count < 2) {
        return true;
    }

    nsk_auto_free struct nsk_wav_segment *items = calloc(
        ctx->segments.count,
        sizeof(*items)
    );
    if (!items) {
        nsk_err("Cannot allocate memory for equal-neighbor segments");
        return false;
    }

    size_t itemcount = 0;
    size_t index = 0;

    while (index < ctx->segments.count) {
        const size_t start = index;
        size_t end = index + 1;

        while (
            end < ctx->segments.count &&
            nsk_wav_sg_isequal(
                &ctx->segments.list[end - 1],
                &ctx->segments.list[end]
            )
        ) {
            end++;
        }

        if (end - start == 1) {
            items[itemcount++] = ctx->segments.list[start];
            index = end;
            continue;
        }

        const bool pulse25 =
            ctx->segments.list[start].active &&
            (
                fabs(ctx->segments.list[start].duty - 25.0) <=
                nsk_options_program.profile.segments
                    .mergedutyequalitythreshold ||
                fabs(ctx->segments.list[start].duty - 75.0) <=
                nsk_options_program.profile.segments
                    .mergedutyequalitythreshold
            );

        if (pulse25) {
            for (size_t i = start; i < end; i++) {
                items[itemcount++] = ctx->segments.list[i];
            }

            index = end;
            continue;
        }

        struct nsk_wav_segment merged = nsk_wav_sg_fromframes(
            wav, ctx,
            ctx->segments.list[start].framestart,
            ctx->segments.list[end - 1].frameend
        );

        if (!nsk_wav_sg_recalc(wav, ctx, &merged)) {
            return false;
        }

        bool ignored = false;

        if (!nsk_wav_sg_segment_template_override(wav, ctx, &merged, &ignored)) {
            return false;
        }

        items[itemcount++] = merged;
        *changed = true;
        index = end;
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
