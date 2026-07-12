#include <stdlib.h>

#include <nsk_util_meta.h>

#include "nsk_wav_dc_dense.h"
#include "nsk_wav_dc_score.h"
#include "nsk_wav_dc_segments.h"
#include "../segments/nsk_wav_sg_pipeline.h"
#include "../../../../arguments/nsk_args_options.h"

/*!
 * \brief  Calculates strict-mark density for dense-direct selection.
 *
 * \param[in] marks  Decoder mark evidence
 * \return Strict-mark ratio over the decoded frame range
 */
double nsk_wav_dc_dense_ratio(
    const struct nsk_wav_dc_marks *marks
) {
    if (!marks || marks->framecount <= 1) {
        return 0.0;
    }

    size_t strict = 0;

    for (size_t frame = 1; frame + 1 < marks->framecount; frame++) {
        if (marks->strictbyframe[frame]) {
            strict++;
        }
    }

    return (double)strict / (marks->framecount - 1);
}

/*!
 * \brief  Checks whether whole-file dense-direct decoding should run.
 *
 * \param[in] marks  Decoder mark evidence
 * \return True if strict-mark density enables dense-direct decoding
 */
bool nsk_wav_dc_dense_should_run(
    const struct nsk_wav_dc_marks *marks
) {
    return
        nsk_options_program.profile.decoder.adaptivedenseenabled &&
        nsk_wav_dc_dense_ratio(marks) >=
            nsk_options_program.profile.decoder.adaptivedenseboundaryratio;
}

/*!
 * \brief  Builds a whole-file dense-direct segment hypothesis.
 *
 * \param[in]      wav    Source WAV data
 * \param[in,out]  ctx    Processing context receiving segment hypothesis
 * \param[in]      marks  Decoder mark evidence
 * \return True if dense-direct decoding completed successfully
 */
bool nsk_wav_dc_dense_direct(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx *ctx,
    const struct nsk_wav_dc_marks *marks
) {
    if (!marks || marks->framecount == 0) {
        return true;
    }

    const size_t maxframe = marks->framecount - 1;
    size_t boundarycount = 2;

    for (size_t frame = 1; frame < maxframe; frame++) {
        if (marks->strictbyframe[frame]) {
            boundarycount++;
        }
    }

    nsk_auto_free size_t *boundaries = calloc(
        boundarycount,
        sizeof(*boundaries)
    );

    if (!boundaries) {
        nsk_err("Cannot allocate dense-direct boundaries");
        return false;
    }

    size_t write = 0;
    boundaries[write++] = 0;

    for (size_t frame = 1; frame < maxframe; frame++) {
        if (marks->strictbyframe[frame]) {
            boundaries[write++] = frame;
        }
    }

    boundaries[write++] = maxframe;

    const struct nsk_wav_sg_pipeline options = {
        .merge = false,
        .refiners = true,
        .densedirect = true
    };

    return nsk_wav_sg_build_boundaries(
        wav, ctx,
        boundaries,
        write,
        &options
    );
}

/*!
 * \brief  Arbitrates between local-hybrid and all-dense segment hypotheses.
 *
 * \param[in]      wav        Source WAV data
 * \param[in,out]  ctx        Processing context receiving the chosen hypothesis
 * \param[in]      intervals  Snapped sparse decoder intervals
 * \param[in]      marks      Decoder mark evidence
 * \param[in]      regions    Local dense regions
 * \return True if arbitration completed successfully
 */
bool nsk_wav_dc_dense_arbitrate_local(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx *ctx,
    const struct nsk_wav_dc_intervals *intervals,
    const struct nsk_wav_dc_marks *marks,
    const struct nsk_wav_dc_regions *regions
) {
    if (!regions || regions->count == 0) {
        return nsk_wav_dc_segments_from_sparse(wav, ctx, intervals);
    }

    const size_t maxframe = marks && marks->framecount > 0 ?
        marks->framecount - 1 :
        0;

    if (!nsk_wav_dc_segments_from_hybrid(
        wav, ctx,
        intervals,
        marks,
        regions
    )) {
        return false;
    }

    const double localscore = nsk_wav_dc_score_segments(ctx, maxframe);

    if (!nsk_wav_dc_dense_direct(wav, ctx, marks)) {
        return false;
    }

    const double densescore = nsk_wav_dc_score_segments(ctx, maxframe);

    if (
        densescore >
        localscore + nsk_options_program.profile.decoder.densearbitrationmargin
    ) {
        return true;
    }

    return nsk_wav_dc_segments_from_hybrid(
        wav, ctx,
        intervals,
        marks,
        regions
    );
}
