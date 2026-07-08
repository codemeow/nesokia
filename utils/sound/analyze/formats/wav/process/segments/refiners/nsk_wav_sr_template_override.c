#include "nsk_wav_sr_template_override.h"
#include "../nsk_wav_sg_template.h"

/*!
 * \brief  Runs template-fit override over all eligible segments.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[out]  changed  Set to true when the function modifies segment data.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_template_override(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    bool *changed
) {
    *changed = false;

    if (
        ctx->segments.count >
        nsk_options_program.profile.segments.templateresidualmaxsegments
    ) {
        return true;
    }

    for (size_t i = 0; i < ctx->segments.count; i++) {
        if (!nsk_wav_sg_segment_template_override(
            wav, ctx,
            &ctx->segments.list[i],
            changed
        )) {
            return false;
        }
    }

    return true;
}
