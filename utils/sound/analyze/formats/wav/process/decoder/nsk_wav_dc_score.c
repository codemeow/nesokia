#include "nsk_wav_dc_score.h"
#include "../../../../arguments/nsk_args_options.h"

/*!
 * \brief  Scores the currently selected segment hypothesis.
 *
 * \param[in] ctx       Processing context containing the segment hypothesis.
 * \param[in] maxframe  One-past-last decoded frame.
 * \return Hypothesis score; larger values are better.
 */
double nsk_wav_dc_score_segments(
    const struct nsk_wav_ctx *ctx,
    size_t maxframe
) {
    if (!ctx || maxframe == 0 || ctx->segments.count == 0) {
        return -1e9;
    }

    double weightedconfidence = 0.0;
    size_t noteframes = 0;
    size_t unknownframes = 0;

    for (size_t i = 0; i < ctx->segments.count; i++) {
        const struct nsk_wav_segment *segment = &ctx->segments.list[i];
        const size_t frames =
            segment->frameend > segment->framestart ?
            segment->frameend - segment->framestart :
            0;

        if (segment->active && segment->midi >= 0) {
            noteframes += frames;
            weightedconfidence += segment->confidence * frames;
            continue;
        }

        if (segment->active) {
            unknownframes += frames;
        }
    }

    const double confidence =
        weightedconfidence / (double)(noteframes > 0 ? noteframes : 1);
    const double fragmentation =
        ctx->segments.count > 0 ?
        (double)(ctx->segments.count - 1) / (double)maxframe :
        0.0;

    return
        confidence -
        ((double)unknownframes *
            nsk_options_program.profile.decoder.unknownnotepenalty) /
            (double)maxframe -
        fragmentation *
            nsk_options_program.profile.decoder.fragmentationweight;
}
