#include <stdlib.h>

#include <nsk_util_meta.h>

#include "nsk_wav_dc_segments.h"
#include "../segments/nsk_wav_sg_list.h"
#include "../segments/nsk_wav_sg_measure.h"
#include "../segments/nsk_wav_sg_pipeline.h"
#include "../segments/refiners/nsk_wav_sr_repair_dense_regions.h"
#include "../../nsk_wav_midi.h"

/*!
 * \brief  Compares two frame boundaries for sorting.
 *
 * \param[in] left   Left frame boundary.
 * \param[in] right  Right frame boundary.
 * \return Negative, zero, or positive comparison result.
 */
static int _dc_segments_cmp_size(
    const void *left,
    const void *right
) {
    const size_t a = *(const size_t *)left;
    const size_t b = *(const size_t *)right;

    return (a > b) - (a < b);
}

/*!
 * \brief  Appends one unsorted boundary to a temporary list.
 *
 * \param[in,out] boundaries  Boundary list pointer to update.
 * \param[in,out] count       Current boundary count.
 * \param[in]     frame       Boundary frame to append.
 * \return True if the boundary was appended successfully.
 */
static bool _dc_segments_boundary_append(
    size_t **boundaries,
    size_t *count,
    size_t frame
) {
    __typeof__(*boundaries) ptr = realloc(
        *boundaries,
        sizeof(**boundaries) * (*count + 1)
    );

    if (!ptr) {
        nsk_err("Cannot allocate hybrid segment boundaries");
        return false;
    }

    *boundaries = ptr;
    (*boundaries)[(*count)++] = frame;

    return true;
}

/*!
 * \brief  Sorts a boundary list and removes duplicate frames in place.
 *
 * \param[in,out] boundaries  Boundary list to compact.
 * \param[in,out] count       Boundary count to update.
 */
static void _dc_segments_boundary_compact(
    size_t *boundaries,
    size_t *count
) {
    if (*count == 0) {
        return;
    }

    qsort(boundaries, *count, sizeof(*boundaries), _dc_segments_cmp_size);

    size_t write = 1;

    for (size_t read = 1; read < *count; read++) {
        if (boundaries[read] != boundaries[write - 1]) {
            boundaries[write++] = boundaries[read];
        }
    }

    *count = write;
}

/*!
 * \brief  Validates snapped sparse intervals before segment construction.
 *
 * \param[in] intervals  Snapped sparse decoder intervals
 * \return True if intervals are contiguous and non-empty
 */
static bool _dc_segments_validate(
    const struct nsk_wav_dc_intervals *intervals
) {
    if (!intervals) {
        return false;
    }

    if (intervals->count == 0) {
        return true;
    }

    if (intervals->list[0].framestart != 0) {
        nsk_err("Sparse interval hypothesis must start at frame zero");
        return false;
    }

    for (size_t i = 0; i < intervals->count; i++) {
        const struct nsk_wav_dc_interval *interval = &intervals->list[i];

        if (interval->framestart >= interval->frameend) {
            nsk_err("Sparse interval hypothesis contains an empty interval");
            return false;
        }

        if (
            i > 0 &&
            interval->framestart != intervals->list[i - 1].frameend
        ) {
            nsk_err("Sparse interval hypothesis must be contiguous");
            return false;
        }
    }

    return true;
}

/*!
 * \brief  Copies sparse interval note/rest state to a measured segment.
 *
 * \param[in]      wav       Source WAV data
 * \param[in,out]  ctx       Processing context
 * \param[in]      interval  Sparse interval state
 * \param[in,out]  segment   Segment receiving state
 * \return True if state was copied successfully
 */
static bool _dc_segments_apply_state(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx *ctx,
    const struct nsk_wav_dc_interval *interval,
    struct nsk_wav_segment *segment
) {
    if (interval->midi < 0) {
        segment->active = false;
        segment->midi = -1;
        segment->frequency = 0.0;
        segment->volume = 0.0;
        segment->duty = 0.0;
        segment->confidence = 0.0;
        segment->harmonicratio = 0.0;
        return true;
    }

    segment->active = true;
    segment->midi = interval->midi;
    segment->frequency = nsk_midi_tofreq(interval->midi);

    if (!nsk_wav_sg_volume(wav, ctx, segment)) {
        return false;
    }

    return true;
}

/*!
 * \brief  Builds a measured segment hypothesis from sparse decoder intervals.
 *
 * \param[in]      wav        Source WAV data
 * \param[in,out]  ctx        Processing context receiving segment hypothesis
 * \param[in]      intervals  Snapped sparse decoder intervals
 * \return True if segment hypothesis was built successfully
 */
bool nsk_wav_dc_segments_from_sparse(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx *ctx,
    const struct nsk_wav_dc_intervals *intervals
) {
    if (!_dc_segments_validate(intervals)) {
        return false;
    }

    if (intervals->count == 0) {
        nsk_wav_sg_clear(ctx);
        return true;
    }

    nsk_auto_free size_t *boundaries = calloc(
        intervals->count + 1,
        sizeof(*boundaries)
    );

    if (!boundaries) {
        nsk_err("Cannot allocate sparse segment boundaries");
        return false;
    }

    boundaries[0] = intervals->list[0].framestart;

    for (size_t i = 0; i < intervals->count; i++) {
        boundaries[i + 1] = intervals->list[i].frameend;
    }

    if (!nsk_wav_sg_create_boundaries(
        wav, ctx,
        boundaries,
        intervals->count + 1
    )) {
        return false;
    }

    if (ctx->segments.count != intervals->count) {
        nsk_err("Sparse segment hypothesis count mismatch");
        return false;
    }

    for (size_t i = 0; i < ctx->segments.count; i++) {
        if (!nsk_wav_sg_p2p(wav, ctx, &ctx->segments.list[i])) {
            return false;
        }

        if (!_dc_segments_apply_state(
            wav,
            ctx,
            &intervals->list[i],
            &ctx->segments.list[i]
        )) {
            return false;
        }
    }

    return true;
}

/*!
 * \brief  Builds a local-hybrid segment hypothesis from sparse and dense boundaries.
 *
 * \param[in]      wav        Source WAV data
 * \param[in,out]  ctx        Processing context receiving segment hypothesis
 * \param[in]      intervals  Snapped sparse decoder intervals
 * \param[in]      marks      Decoder mark evidence
 * \param[in]      regions    Local dense regions
 * \return True if segment hypothesis was built successfully
 */
bool nsk_wav_dc_segments_from_hybrid(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx *ctx,
    const struct nsk_wav_dc_intervals *intervals,
    const struct nsk_wav_dc_marks *marks,
    const struct nsk_wav_dc_regions *regions
) {
    if (!_dc_segments_validate(intervals)) {
        return false;
    }

    if (!marks || !regions || regions->count == 0) {
        return nsk_wav_dc_segments_from_sparse(wav, ctx, intervals);
    }

    if (intervals->count == 0) {
        nsk_wav_sg_clear(ctx);
        return true;
    }

    const size_t maxframe = marks->framecount > 0 ? marks->framecount - 1 : 0;
    size_t *boundaries = NULL;
    size_t boundarycount = 0;
    bool result = false;

    if (
        !_dc_segments_boundary_append(&boundaries, &boundarycount, 0) ||
        !_dc_segments_boundary_append(&boundaries, &boundarycount, maxframe)
    ) {
        goto cleanup;
    }

    for (size_t i = 0; i < intervals->count; i++) {
        const struct nsk_wav_dc_interval *interval = &intervals->list[i];

        if (
            interval->framestart != 0 &&
            interval->framestart != maxframe &&
            !nsk_wav_dc_regions_contains(regions, interval->framestart) &&
            !_dc_segments_boundary_append(
                &boundaries,
                &boundarycount,
                interval->framestart
            )
        ) {
            goto cleanup;
        }

        if (
            interval->frameend != 0 &&
            interval->frameend != maxframe &&
            !nsk_wav_dc_regions_contains(regions, interval->frameend) &&
            !_dc_segments_boundary_append(
                &boundaries,
                &boundarycount,
                interval->frameend
            )
        ) {
            goto cleanup;
        }
    }

    for (size_t i = 0; i < regions->count; i++) {
        const struct nsk_wav_dc_region *region = &regions->list[i];

        if (
            !_dc_segments_boundary_append(
                &boundaries,
                &boundarycount,
                region->framestart
            ) ||
            !_dc_segments_boundary_append(
                &boundaries,
                &boundarycount,
                region->frameend
            )
        ) {
            goto cleanup;
        }

        for (
            size_t frame = region->framestart + 1;
            frame < region->frameend && frame < marks->framecount;
            frame++
        ) {
            if (
                marks->strictbyframe[frame] &&
                !_dc_segments_boundary_append(
                    &boundaries,
                    &boundarycount,
                    frame
                )
            ) {
                goto cleanup;
            }
        }
    }

    _dc_segments_boundary_compact(boundaries, &boundarycount);

    const struct nsk_wav_sg_pipeline options = {
        .merge = false,
        .refiners = false,
        .densedirect = false
    };

    if (!nsk_wav_sg_build_boundaries(
        wav, ctx,
        boundaries,
        boundarycount,
        &options
    )) {
        goto cleanup;
    }

    bool changed = false;

    if (!nsk_wav_sg_repair_dense_chromatic_sweep(wav, ctx, &changed)) {
        goto cleanup;
    }

    result = true;

cleanup:
    free(boundaries);

    return result;
}
