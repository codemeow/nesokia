#include "nsk_wav_sr_reclassify_equal_neighbors.h"
#include "../nsk_wav_sg_list.h"
#include "../nsk_wav_sg_classify.h"
#include "../nsk_wav_sg_template.h"

/*!
 * \brief  Checks whether neighboring segments belong to the same state and note run.
 *
 * \param[in]  left  Left segment.
 * \param[in]  right  Right segment.
 * \return True when the segments should be analyzed as one run.
 */
static bool _sr_equal_neighbor_same_run(
    const struct nsk_wav_segment *left,
    const struct nsk_wav_segment *right
) {
    if (left->frameend != right->framestart) {
        return false;
    }

    if (left->active != right->active) {
        return false;
    }

    if (!left->active) {
        return true;
    }

    return left->midi == right->midi;
}

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
            _sr_equal_neighbor_same_run(
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
