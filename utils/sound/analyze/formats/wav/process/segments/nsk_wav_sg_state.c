#include "nsk_wav_sg_state.h"

/*!
 * \brief  Calculates the accepted dense-mark ratio.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \return Calculated floating-point value.
 */
double nsk_wav_sg_dense_ratio(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
) {
    (void)wav;

    if (ctx->marks.count == 0) {
        return 0.0;
    }

    size_t dense = 0;

    for (size_t i = 0; i < ctx->marks.count; i++) {
        if (ctx->marks.list[i].indense) {
            dense++;
        }
    }

    return (double)dense / ctx->marks.count;
}

/*!
 * \brief  Calculates the accepted-mark ratio.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \return Calculated floating-point value.
 */
double nsk_wav_sg_accepted_ratio(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
) {
    (void)wav;

    if (ctx->marks.count == 0) {
        return 0.0;
    }

    size_t accepted = 0;

    for (size_t i = 0; i < ctx->marks.count; i++) {
        if (ctx->marks.list[i].accepted) {
            accepted++;
        }
    }

    return (double)accepted / ctx->marks.count;
}

/*!
 * \brief  Checks whether the mark set is dense enough for direct dense processing.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_isdensedirect(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
) {
    return
        nsk_wav_sg_accepted_ratio(wav, ctx) >=
        nsk_options_program.profile.segments.densedirectminacceptedratio;
}
