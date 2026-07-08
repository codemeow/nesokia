#include <nsk_util_meta.h>

#include "nsk_wav_segments.h"
#include "segments/nsk_wav_sg_classify.h"
#include "segments/nsk_wav_sg_list.h"
#include "segments/nsk_wav_sg_measure.h"
#include "segments/nsk_wav_sg_state.h"
#include "segments/refiners/nsk_wav_sr_merge_zero_delta_neighbors.h"
#include "segments/refiners/nsk_wav_sr_reclassify_dense_singleframe.h"
#include "segments/refiners/nsk_wav_sr_reclassify_equal_neighbors.h"
#include "segments/refiners/nsk_wav_sr_reclassify_outer_pulse_width.h"
#include "segments/refiners/nsk_wav_sr_refine_measurements.h"
#include "segments/refiners/nsk_wav_sr_repair_dense_regions.h"
#include "segments/refiners/nsk_wav_sr_split_candidate_marks.h"
#include "segments/refiners/nsk_wav_sr_split_confident_frame_grid.h"
#include "segments/refiners/nsk_wav_sr_split_edge_confirmed_twoframe.h"
#include "segments/refiners/nsk_wav_sr_split_pulse_width_edgeframes.h"
#include "segments/refiners/nsk_wav_sr_template_override.h"

/*!
 * \brief  Processes the wav segments
 *
 * \param[in]      wav   Source WAV data
 * \param[in,out]  ctx   Processing context whose segments are processed
 * \return True if the processing completed successfully
 */
bool nsk_wav_segment(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
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

    if (!nsk_wav_sg_create(wav, ctx)) {
        return false;
    }

    if (!nsk_wav_sg_calcsilence(wav, ctx)) {
        return false;
    }

    for (size_t sgi = 0; sgi < ctx->segments.count; sgi++) {
        for (size_t sti = 0; sti < NSK_SIZE(_table); sti++) {
            if (!_table[sti].step(wav, ctx, &ctx->segments.list[sgi])) {
                return false;
            }
        }
    }

    if (!nsk_wav_sg_isdensedirect(wav, ctx)) {
        if (!nsk_wav_sg_merge(wav, ctx)) {
            return false;
        }
    }

    for (size_t i = 0; i < NSK_SIZE(_refiners); i++) {
        bool changed = false;

        if (!_refiners[i].refiner(wav, ctx, &changed)) {
            return false;
        }

        if (changed) {
            if (_refiners[i].mergeafter && !nsk_wav_sg_isdensedirect(wav, ctx)) {
                if (!nsk_wav_sg_merge(wav, ctx)) {
                    return false;
                }
            }
        }
    }

    return true;
}
