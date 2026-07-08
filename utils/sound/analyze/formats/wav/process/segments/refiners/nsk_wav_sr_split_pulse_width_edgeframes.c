#include "nsk_wav_sr_split_pulse_width_edgeframes.h"
#include "../nsk_wav_sg_template.h"
#include "../nsk_wav_sg_list.h"
#include "../nsk_wav_sg_state.h"
#include "../nsk_wav_sg_pulsewidth.h"
#include "../nsk_wav_sg_classify.h"

/*!
 * \brief  Splits first frames in short low-note pulse-width cases.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[out]  changed  Set to true when the function modifies segment data.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_split_pulse_width_edgeframes(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    bool *changed
) {
    *changed = false;

    if (!nsk_wav_sg_isdensedirect(wav, ctx)) {
        return true;
    }

    for (size_t i = 0; i < ctx->segments.count; i++) {
        const struct nsk_wav_segment segment = ctx->segments.list[i];
        const size_t framecount = segment.frameend - segment.framestart;

        if (
            !segment.active ||
            framecount < 2 ||
            framecount >
            nsk_options_program.profile.segments.pulsewidthsplitmaxframes
        ) {
            continue;
        }

        struct _segment_pulsewidth pitch = {0};

        if (!nsk_wav_sg_pulse_width_note_oneframe(
            wav, ctx,
            segment.framestart,
            &pitch
        )) {
            return false;
        }

        struct nsk_wav_segment remaining = nsk_wav_sg_fromframes(
            wav, ctx,
            segment.framestart + 1,
            segment.frameend
        );

        if (!nsk_wav_sg_recalc(wav, ctx, &remaining)) {
            return false;
        }

        bool remainingchanged = false;

        if (!nsk_wav_sg_segment_template_override(wav, ctx, &remaining, &remainingchanged)) {
            return false;
        }

        if (
            remaining.active &&
            remaining.midi != segment.midi &&
            pitch.found &&
            pitch.midi < segment.midi &&
            fabs(pitch.duty - 75.0) <=
            nsk_options_program.profile.segments.mergedutyequalitythreshold
        ) {
            struct _segment_templatefit fit = {0};
            int midis[] = {
                segment.midi
            };

            if (!nsk_wav_sg_best_template_fit(
                wav, ctx,
                remaining.samplestart,
                remaining.sampleend,
                midis,
                NSK_SIZE(midis),
                &fit
            )) {
                return false;
            }

            if (
                fit.found &&
                fit.score >=
                nsk_options_program.profile.segments
                    .pulsewidthsplitremainingminconfidence
            ) {
                remaining.midi = segment.midi;
                remaining.frequency = nsk_midi_tofreq(segment.midi);
                remaining.confidence = NSK_MAX(remaining.confidence, fit.score);
                remaining.duty = fit.duty;
            }
        }

        if (
            !pitch.found ||
            pitch.midi == segment.midi ||
            pitch.midi >
            nsk_options_program.profile.segments.pulsewidthsplitmaxmidi ||
            pitch.templatescore <
            nsk_options_program.profile.segments.pulsewidthsplitmintemplatescore ||
            pitch.templatemargin <
            nsk_options_program.profile.segments.pulsewidthsplitmintemplatemargin ||
            !remaining.active ||
            remaining.midi != segment.midi ||
            remaining.confidence <
            nsk_options_program.profile.segments.pulsewidthsplitremainingminconfidence
        ) {
            continue;
        }

        struct nsk_wav_segment first = nsk_wav_sg_fromframes(
            wav, ctx,
            segment.framestart,
            segment.framestart + 1
        );

        if (!nsk_wav_sg_recalc(wav, ctx, &first)) {
            return false;
        }

        first.active = true;
        first.midi = pitch.midi;
        first.frequency = nsk_midi_tofreq(pitch.midi);
        first.confidence = NSK_MAX(
            first.confidence,
            NSK_MAX(pitch.confidence, pitch.templatescore)
        );
        first.duty = pitch.duty;

        const struct nsk_wav_segment items[] = {
            first,
            remaining
        };

        if (!nsk_wav_sg_replace_range(wav, ctx, i, 1, items, NSK_SIZE(items))) {
            return false;
        }

        *changed = true;
    }

    return true;
}
