#include "nsk_wav_sg_list.h"
#include "nsk_wav_sg_classify.h"
#include "nsk_wav_sg_template.h"

/*!
 * \brief  Converts a frame position to the nearest source sample index.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  frame  Frame index or frame position.
 * \return Calculated size value.
 */
size_t nsk_wav_sg_frame_sample(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    double frame
) {
    (void)ctx;

    return lround(
        frame *
        wav->format.samplerate /
        nsk_options_program.profile.boundary.grid.fps
    );
}

/*!
 * \brief  Appends a segment described by frame bounds to the segment list.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  framestart  Inclusive frame start.
 * \param[in]  frameend  Exclusive frame end.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_appendframes(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    size_t          framestart,
    size_t          frameend
) {
    __typeof__((ctx->segments.list)) ptr =
        realloc(
            ctx->segments.list,
            (ctx->segments.count + 1) * sizeof(ctx->segments.list[0])
        );

    if (!ptr) {
        nsk_err(
            "Cannot reallocate the memory for the segments list"
        );
        return false;
    }

    ctx->segments.list = ptr;
    ctx->segments.list[ctx->segments.count++] =
    (__typeof__(ctx->segments.list[0])) {
        .framestart  = framestart,
        .frameend    = frameend,
        .samplestart = nsk_wav_sg_frame_sample(wav, ctx, framestart),
        .sampleend   = nsk_wav_sg_frame_sample(wav, ctx, frameend)
    };

    return true;
}

/*!
 * \brief  Appends a segment covering a contiguous accepted mark range.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  start  Inclusive mark range start.
 * \param[in]  end  Exclusive mark range end.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_append_mark_range(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    size_t          start,
    size_t          end
) {
    return nsk_wav_sg_appendframes(
        wav, ctx,
        ctx->marks.list[start].frame,
        ctx->marks.list[end].frame
    );
}

/*!
 * \brief  Builds the initial segment list from accepted marks.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_create(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
) {
    size_t acceptedstart = 0;

    for (size_t i = 0; i < ctx->marks.count; i++) {
        if (!ctx->marks.list[i].accepted) {
            continue;
        }

        if (!nsk_wav_sg_append_mark_range(
            wav, ctx,
            acceptedstart,
            i
        )) {
            return false;
        }

        acceptedstart = i;
    }

    if (acceptedstart != ctx->marks.count) {
        if (!nsk_wav_sg_appendframes(
            wav, ctx,
            ctx->marks.list[acceptedstart].frame,
            ctx->marks.count
        )) {
            return false;
        }
    }

    return true;
}

/*!
 * \brief  Creates a segment value from frame bounds.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  framestart  Inclusive frame start.
 * \param[in]  frameend  Exclusive frame end.
 * \return Constructed segment value.
 */
struct nsk_wav_segment nsk_wav_sg_fromframes(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    size_t framestart,
    size_t frameend
) {
    (void)ctx;

    const double samplerate = wav->format.samplerate;
    const double fps = nsk_options_program.profile.boundary.grid.fps;

    return (struct nsk_wav_segment) {
        .framestart  = framestart,
        .frameend    = frameend,
        .samplestart = lround((double)framestart * samplerate / fps),
        .sampleend   = lround((double)frameend * samplerate / fps),
        .midi        = -1
    };
}

/*!
 * \brief  Replaces a range of segments with the supplied segment items.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  index  First segment index to replace or repair.
 * \param[in]  removecount  Number of existing segments to remove.
 * \param[in]  items  Replacement segment array.
 * \param[in]  itemcount  Number of replacement items.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_replace_range(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    size_t index,
    size_t removecount,
    const struct nsk_wav_segment *items,
    size_t itemcount
) {
    (void)wav;

    const size_t newcount = ctx->segments.count - removecount + itemcount;
    __typeof__(ctx->segments.list) list = NULL;

    if (newcount > 0) {
        list = malloc(newcount * sizeof(*list));

        if (!list) {
            nsk_err("Cannot allocate memory for segment replacement");
            return false;
        }

        memcpy(
            list,
            ctx->segments.list,
            index * sizeof(*list)
        );

        if (itemcount > 0) {
            memcpy(
                &list[index],
                items,
                itemcount * sizeof(*list)
            );
        }

        memcpy(
            &list[index + itemcount],
            &ctx->segments.list[index + removecount],
            (ctx->segments.count - index - removecount) * sizeof(*list)
        );
    }

    free(ctx->segments.list);
    ctx->segments.list = list;
    ctx->segments.count = newcount;

    return true;
}

/*!
 * \brief  Checks whether two neighboring segments are equal for merge purposes.
 *
 * \param[in,out]  left  Left segment or left numeric value.
 * \param[in]  right  Right segment or right numeric value.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_isequal(
    const struct nsk_wav_segment *left,
    const struct nsk_wav_segment *right
) {
    if (left->active != right->active) {
        return false;
    }

    if (!left->active) {
        return true;
    }

    if (left->midi != right->midi) {
        return false;
    }

    if (
        fabs(left->volume - right->volume) >
        nsk_options_program.profile.segments.mergevolumeequalitythreshold
    ) {
        return false;
    }

    if (
        fabs(left->duty - right->duty) >
        nsk_options_program.profile.segments.mergedutyequalitythreshold
    ) {
        return false;
    }

    return true;
}

/*!
 * \brief  Calculates a weighted average for two segment measurements.
 *
 * \param[in,out]  left  Left segment or left numeric value.
 * \param[in]  leftweight  Weight for the left value.
 * \param[in]  right  Right segment or right numeric value.
 * \param[in]  rightweight  Weight for the right value.
 * \return Calculated floating-point value.
 */
double nsk_wav_sg_weightedaverage(
    double left,
    size_t leftweight,
    double right,
    size_t rightweight
) {
    const size_t totalweight = leftweight + rightweight;

    if (totalweight == 0) {
        return 0.0;
    }

    return
        (
            left * leftweight +
            right * rightweight
        ) /
        totalweight;
}

/*!
 * \brief  Merges the right segment into the left segment.
 *
 * \param[in,out]  left  Left segment or left numeric value.
 * \param[in]  right  Right segment or right numeric value.
 */
void nsk_wav_sg_mergeitem(
    struct nsk_wav_segment *left,
    const struct nsk_wav_segment *right
) {
    const size_t leftframes = left->frameend - left->framestart;
    const size_t rightframes = right->frameend - right->framestart;

    left->frameend = right->frameend;
    left->sampleend = right->sampleend;
    left->p2p = NSK_MAX(left->p2p, right->p2p);
    left->volume = nsk_wav_sg_weightedaverage(
        left->volume,
        leftframes,
        right->volume,
        rightframes
    );
    left->confidence = NSK_MIN(left->confidence, right->confidence);
    left->harmonicratio = NSK_MIN(left->harmonicratio, right->harmonicratio);
    left->duty = nsk_wav_sg_weightedaverage(
        left->duty,
        leftframes,
        right->duty,
        rightframes
    );
}

/*!
 * \brief  Merges adjacent equivalent segments in place.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_merge(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
) {
    if (ctx->segments.count < 2) {
        return true;
    }

    bool *merged = calloc(
        ctx->segments.count,
        sizeof(*merged)
    );
    if (!merged) {
        nsk_err("Cannot allocate memory for merged segment flags");
        return false;
    }

    size_t write = 0;

    for (size_t read = 1; read < ctx->segments.count; read++) {
        if (nsk_wav_sg_isequal(
            &ctx->segments.list[write],
            &ctx->segments.list[read]
        )) {
            nsk_wav_sg_mergeitem(
                &ctx->segments.list[write],
                &ctx->segments.list[read]
            );
            merged[write] = true;
            continue;
        }

        write++;

        if (write != read) {
            ctx->segments.list[write] = ctx->segments.list[read];
        }

        merged[write] = false;
    }

    ctx->segments.count = write + 1;

    for (size_t i = 0; i < ctx->segments.count; i++) {
        if (!merged[i]) {
            continue;
        }

        if (!nsk_wav_sg_recalc(wav, ctx, &ctx->segments.list[i])) {
            free(merged);
            return false;
        }

        bool ignored = false;

        if (!nsk_wav_sg_segment_template_override(wav, ctx, &ctx->segments.list[i], &ignored)) {
            free(merged);
            return false;
        }
    }

    free(merged);

    return true;
}
