#include "nsk_wav_decoder.h"

#include "decoder/nsk_wav_dc_boundaries.h"
#include "decoder/nsk_wav_dc_dense.h"
#include "decoder/nsk_wav_dc_framecosts.h"
#include "decoder/nsk_wav_dc_marks.h"
#include "decoder/nsk_wav_dc_notes.h"
#include "decoder/nsk_wav_dc_regions.h"
#include "decoder/nsk_wav_dc_segments.h"
#include "decoder/nsk_wav_dc_snap.h"
#include "decoder/nsk_wav_dc_sparse.h"

/*!
 * \brief  Decodes classified waveform evidence into final note/rest events
 *
 * \param[in]      wav   Source WAV data
 * \param[in,out]  ctx   Processing context whose final events are decoded
 * \return True if decoding completed successfully
 */
bool nsk_wav_decoder(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
) {
    struct nsk_wav_dc_framecosts framecosts = {0};
    struct nsk_wav_dc_marks marks = {0};
    struct nsk_wav_dc_regions regions = {0};
    struct nsk_wav_dc_boundaries boundaries = {0};
    struct nsk_wav_dc_intervals intervals = {0};
    bool result = false;

    if (!nsk_wav_dc_framecosts_build(wav, ctx, &framecosts)) {
        goto cleanup;
    }

    if (!nsk_wav_dc_marks_build(wav, ctx, &marks)) {
        goto cleanup;
    }

    if (!nsk_wav_dc_regions_find(&marks, &regions)) {
        goto cleanup;
    }

    if (nsk_wav_dc_dense_should_run(&marks)) {
        if (!nsk_wav_dc_dense_direct(wav, ctx, &marks)) {
            goto cleanup;
        }

        if (!nsk_wav_dc_notes_from_segments(wav, ctx)) {
            goto cleanup;
        }

        result = true;
        goto cleanup;
    }

    if (!nsk_wav_dc_boundaries_build(&marks, &boundaries)) {
        goto cleanup;
    }

    if (!nsk_wav_dc_sparse_decode(&framecosts, &boundaries, &intervals)) {
        goto cleanup;
    }

    if (!nsk_wav_dc_snap_apply(&framecosts, &intervals)) {
        goto cleanup;
    }

    if (regions.count > 0) {
        if (!nsk_wav_dc_dense_arbitrate_local(
            wav,
            ctx,
            &intervals,
            &marks,
            &regions
        )) {
            goto cleanup;
        }
    } else {
        if (!nsk_wav_dc_segments_from_sparse(wav, ctx, &intervals)) {
            goto cleanup;
        }
    }

    if (!nsk_wav_dc_notes_from_segments(wav, ctx)) {
        goto cleanup;
    }

    result = true;

cleanup:
    nsk_wav_dc_intervals_free(&intervals);
    nsk_wav_dc_boundaries_free(&boundaries);
    nsk_wav_dc_regions_free(&regions);
    nsk_wav_dc_marks_free(&marks);
    nsk_wav_dc_framecosts_free(&framecosts);

    return result;
}
