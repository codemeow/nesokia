#include "nsk_wav_sr_refine_measurements.h"
#include "../nsk_wav_sg_measure.h"

/*!
 * \brief  Refreshes final segment measurements over their final bounds.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[out]  changed  Set to true when the function modifies segment data.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_refine_measurements(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    bool *changed
) {
    *changed = false;

    for (size_t i = 0; i < ctx->segments.count; i++) {
        const struct nsk_wav_segment before = ctx->segments.list[i];

        if (!nsk_wav_sg_p2p(wav, ctx, &ctx->segments.list[i])) {
            return false;
        }

        if (!nsk_wav_sg_active(wav, ctx, &ctx->segments.list[i])) {
            return false;
        }

        if (!ctx->segments.list[i].active) {
            ctx->segments.list[i].midi = -1;
            ctx->segments.list[i].frequency = 0.0;
            ctx->segments.list[i].confidence = 0.0;
            ctx->segments.list[i].harmonicratio = 0.0;
            ctx->segments.list[i].volume = 0.0;
            ctx->segments.list[i].duty = 0.0;

        } else {
            if (!nsk_wav_sg_volume(wav, ctx, &ctx->segments.list[i])) {
                return false;
            }

            double duty = 0.0;

            if (nsk_wav_sg_estimate_folded_duty(wav, ctx, &ctx->segments.list[i], &duty)) {
                const bool pulse25 =
                    fabs(ctx->segments.list[i].duty - 25.0) <=
                    nsk_options_program.profile.segments
                        .mergedutyequalitythreshold ||
                    fabs(ctx->segments.list[i].duty - 75.0) <=
                    nsk_options_program.profile.segments
                        .mergedutyequalitythreshold;
                const bool low125guard =
                    duty == 12.5 &&
                    ctx->segments.list[i].frameend -
                    ctx->segments.list[i].framestart <= 1 &&
                    ctx->segments.list[i].midi <=
                    nsk_options_program.profile.segments
                        .foldedduty125guardmaxmidi &&
                    pulse25;

                if (!low125guard) {
                    ctx->segments.list[i].duty = duty;
                }
            }
        }

        if (
            before.active != ctx->segments.list[i].active ||
            before.midi != ctx->segments.list[i].midi ||
            before.frameend != ctx->segments.list[i].frameend ||
            before.sampleend != ctx->segments.list[i].sampleend ||
            before.p2p != ctx->segments.list[i].p2p ||
            before.volume != ctx->segments.list[i].volume ||
            before.confidence != ctx->segments.list[i].confidence ||
            before.harmonicratio != ctx->segments.list[i].harmonicratio ||
            before.duty != ctx->segments.list[i].duty
        ) {
            *changed = true;
        }
    }

    return true;
}
