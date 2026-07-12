#include "nsk_wav_sg_pipeline.h"

#include "nsk_wav_sg_classify.h"
#include "nsk_wav_sg_list.h"
#include "nsk_wav_sg_measure.h"
#include "nsk_wav_sg_state.h"
#include "refiners/nsk_wav_sr_merge_zero_delta_neighbors.h"
#include "refiners/nsk_wav_sr_reclassify_dense_singleframe.h"
#include "refiners/nsk_wav_sr_reclassify_equal_neighbors.h"
#include "refiners/nsk_wav_sr_reclassify_outer_pulse_width.h"
#include "refiners/nsk_wav_sr_refine_measurements.h"
#include "refiners/nsk_wav_sr_repair_dense_regions.h"
#include "refiners/nsk_wav_sr_split_candidate_marks.h"
#include "refiners/nsk_wav_sr_split_confident_frame_grid.h"
#include "refiners/nsk_wav_sr_split_edge_confirmed_twoframe.h"
#include "refiners/nsk_wav_sr_split_pulse_width_edgeframes.h"
#include "refiners/nsk_wav_sr_template_override.h"

/*!
 * \brief  Processes the current segment list as one hypothesis.
 *
 * \param[in]      wav      Source WAV data.
 * \param[in,out]  ctx      Processing context used by the segment pipeline.
 * \param[in]      options  Segment pipeline options.
 * \return True if the hypothesis was processed successfully.
 */
bool nsk_wav_sg_process(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    const struct nsk_wav_sg_pipeline *options
) {
    /*!
     * \brief  Initial per-segment processing step table.
     */
    static const struct {
        bool (*step)(
            const struct nsk_wav *wav,
            struct nsk_wav_ctx  *ctx,
            struct nsk_wav_segment *segment
        );
    } _table[] = {
        {
            .step = nsk_wav_sg_p2p
        },
        {
            .step = nsk_wav_sg_active
        },
        {
            .step = nsk_wav_sg_volume
        },
        {
            .step = nsk_wav_sg_classify
        }
    };
    /*!
     * \brief  Ordered segment refiner pipeline.
     */
    static const struct {
        bool mergeafter;
        bool (*refiner)(
            const struct nsk_wav *wav,
            struct nsk_wav_ctx  *ctx,
            bool *changed
        );
    } _refiners[] = {
        {
            .mergeafter = true,
            .refiner = nsk_wav_sg_split_candidate_marks
        },
        {
            .mergeafter = true,
            .refiner = nsk_wav_sg_template_override
        },
        {
            .mergeafter = true,
            .refiner = nsk_wav_sg_reclassify_equal_neighbors
        },
        {
            .mergeafter = true,
            .refiner = nsk_wav_sg_merge_zero_delta_neighbors
        },
        {
            .mergeafter = false,
            .refiner = nsk_wav_sg_split_edge_confirmed_twoframe
        },
        {
            .mergeafter = false,
            .refiner = nsk_wav_sg_split_pulse_width_edgeframes
        },
        {
            .mergeafter = false,
            .refiner = nsk_wav_sg_split_confident_frame_grid
        },
        {
            .mergeafter = true,
            .refiner = nsk_wav_sg_reclassify_dense_singleframe
        },
        {
            .mergeafter = true,
            .refiner = nsk_wav_sg_reclassify_outer_pulse_width
        },
        {
            .mergeafter = true,
            .refiner = nsk_wav_sg_repair_dense_regions
        },
        {
            .mergeafter = false,
            .refiner = nsk_wav_sg_refine_measurements
        }
    };

    const struct nsk_wav_sg_pipeline defaults = {
        .merge       = true,
        .refiners    = true,
        .densedirect = nsk_wav_sg_isdensedirect(wav, ctx)
    };
    const struct nsk_wav_sg_pipeline pipeline =
        options ? *options : defaults;
    const bool olddensedirect = ctx->segments.densedirect;
    bool result = false;

    ctx->segments.densedirect = pipeline.densedirect;

    if (!nsk_wav_sg_calcsilence(wav, ctx)) {
        goto cleanup;
    }

    for (size_t sgi = 0; sgi < ctx->segments.count; sgi++) {
        for (size_t sti = 0; sti < NSK_SIZE(_table); sti++) {
            if (!_table[sti].step(wav, ctx, &ctx->segments.list[sgi])) {
                goto cleanup;
            }
        }
    }

    if (pipeline.merge && !pipeline.densedirect) {
        if (!nsk_wav_sg_merge(wav, ctx)) {
            goto cleanup;
        }
    }

    if (!pipeline.refiners) {
        result = true;
        goto cleanup;
    }

    for (size_t i = 0; i < NSK_SIZE(_refiners); i++) {
        bool changed = false;

        if (!_refiners[i].refiner(wav, ctx, &changed)) {
            goto cleanup;
        }

        if (
            changed &&
            pipeline.merge &&
            _refiners[i].mergeafter &&
            !pipeline.densedirect
        ) {
            if (!nsk_wav_sg_merge(wav, ctx)) {
                goto cleanup;
            }
        }
    }

    result = true;

cleanup:
    ctx->segments.densedirect = olddensedirect;
    return result;
}

/*!
 * \brief  Builds and processes a segment hypothesis from accepted marks.
 *
 * \param[in]      wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \return True if the hypothesis was built successfully.
 */
bool nsk_wav_sg_build_marks(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
) {
    if (!nsk_wav_sg_create(wav, ctx)) {
        return false;
    }

    const struct nsk_wav_sg_pipeline options = {
        .merge       = true,
        .refiners    = true,
        .densedirect = nsk_wav_sg_isdensedirect(wav, ctx)
    };

    return nsk_wav_sg_process(wav, ctx, &options);
}

/*!
 * \brief  Builds and processes a segment hypothesis from explicit boundaries.
 *
 * \param[in]      wav         Source WAV data.
 * \param[in,out]  ctx         Processing context used by the segment pipeline.
 * \param[in]      boundaries  Ordered frame boundary list.
 * \param[in]      count       Number of frame boundaries.
 * \param[in]      options     Segment pipeline options.
 * \return True if the hypothesis was built successfully.
 */
bool nsk_wav_sg_build_boundaries(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    const size_t *boundaries,
    size_t count,
    const struct nsk_wav_sg_pipeline *options
) {
    if (!nsk_wav_sg_create_boundaries(wav, ctx, boundaries, count)) {
        return false;
    }

    return nsk_wav_sg_process(wav, ctx, options);
}
