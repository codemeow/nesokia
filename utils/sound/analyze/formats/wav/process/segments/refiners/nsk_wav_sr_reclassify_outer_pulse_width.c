#include "nsk_wav_sr_reclassify_outer_pulse_width.h"
#include "../nsk_wav_sg_template.h"
#include "../nsk_wav_sg_pulsewidth.h"

/*!
 * \brief  Reclassifies short outer segments using pulse-width and template evidence.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[out]  changed  Set to true when the function modifies segment data.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_reclassify_outer_pulse_width(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    bool *changed
) {
    *changed = false;

    for (size_t i = 0; i < ctx->segments.count; i++) {
        struct nsk_wav_segment *segment = &ctx->segments.list[i];
        const size_t framecount = segment->frameend - segment->framestart;

        if (
            !segment->active ||
            framecount < 1 ||
            framecount >
            nsk_options_program.profile.segments.outerpulsemaxframes ||
            !(i == 0 || i + 1 == ctx->segments.count) ||
            segment->midi < 0
        ) {
            continue;
        }

        int candidates[framecount];
        size_t candidatecount = 0;

        for (size_t frame = segment->framestart; frame < segment->frameend; frame++) {
            struct _segment_pulsewidth pulse = {0};

            if (!nsk_wav_sg_pulse_width_note_oneframe(wav, ctx, frame, &pulse)) {
                return false;
            }

            if (
                !pulse.found ||
                pulse.midi >
                nsk_options_program.profile.segments.outerpulsemaxmidi ||
                pulse.templatescore <
                nsk_options_program.profile.segments.outerpulsemintemplatescore ||
                pulse.templatemargin <
                nsk_options_program.profile.segments.outerpulsemintemplatemargin
            ) {
                continue;
            }

            bool seen = false;

            for (size_t ci = 0; ci < candidatecount; ci++) {
                if (candidates[ci] == pulse.midi) {
                    seen = true;
                    break;
                }
            }

            if (!seen) {
                candidates[candidatecount] = pulse.midi;
                candidatecount++;
            }
        }

        if (candidatecount == 0) {
            continue;
        }

        bool currentseen = false;

        for (size_t ci = 0; ci < candidatecount; ci++) {
            if (candidates[ci] == segment->midi) {
                currentseen = true;
                break;
            }
        }

        if (currentseen) {
            continue;
        }

        struct _segment_templatefit current = {0};
        struct _segment_templatefit best = {0};
        int currentmidi[] = {
            segment->midi
        };

        if (!nsk_wav_sg_best_template_fit(
            wav, ctx,
            segment->samplestart,
            segment->sampleend,
            currentmidi,
            NSK_SIZE(currentmidi),
            &current
        )) {
            return false;
        }

        if (!nsk_wav_sg_best_template_fit(
            wav, ctx,
            segment->samplestart,
            segment->sampleend,
            candidates,
            candidatecount,
            &best
        )) {
            return false;
        }

        if (
            !best.found ||
            best.score <
            nsk_options_program.profile.segments.outerpulsemintemplatescore ||
            best.score - current.score <
            nsk_options_program.profile.segments.outerpulseminimprovement
        ) {
            continue;
        }

        segment->midi = best.midi;
        segment->frequency = nsk_midi_tofreq(best.midi);
        segment->confidence = NSK_MAX(segment->confidence, best.score);
        segment->duty = best.duty;
        *changed = true;
    }

    return true;
}
