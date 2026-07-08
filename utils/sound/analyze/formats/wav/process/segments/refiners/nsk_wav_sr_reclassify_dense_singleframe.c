#include "nsk_wav_sr_reclassify_dense_singleframe.h"
#include "../nsk_wav_sg_template.h"
#include "../nsk_wav_sg_state.h"
#include "../nsk_wav_sg_pulsewidth.h"

/*!
 * \brief  Reclassifies dense one-frame segments using pulse-width and template evidence.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[out]  changed  Set to true when the function modifies segment data.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_reclassify_dense_singleframe(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    bool *changed
) {
    *changed = false;

    if (
        nsk_wav_sg_dense_ratio(wav, ctx) <
        nsk_options_program.profile.segments.denseframemindensesratio
    ) {
        return true;
    }

    for (size_t i = 0; i < ctx->segments.count; i++) {
        struct nsk_wav_segment *segment = &ctx->segments.list[i];

        if (
            !segment->active ||
            segment->frameend - segment->framestart != 1 ||
            segment->midi < 0
        ) {
            continue;
        }

        struct _segment_pulsewidth pulse = {0};

        if (!nsk_wav_sg_pulse_width_note_oneframe(
            wav, ctx,
            segment->framestart,
            &pulse
        )) {
            return false;
        }

        if (
            pulse.found &&
            pulse.midi != segment->midi &&
            pulse.midi <=
            nsk_options_program.profile.segments.denseframepulsemaxmidi &&
            pulse.confidence >=
            nsk_options_program.profile.segments.denseframepulseminconfidence &&
            pulse.templatescore >=
            nsk_options_program.profile.segments.denseframepulsemintemplatescore &&
            pulse.templatemargin >=
            nsk_options_program.profile.segments.denseframepulsemargin
        ) {
            segment->midi = pulse.midi;
            segment->frequency = nsk_midi_tofreq(pulse.midi);
            segment->confidence = NSK_MAX(
                segment->confidence,
                NSK_MAX(pulse.confidence, pulse.templatescore)
            );
            segment->duty = pulse.duty;
            *changed = true;
            continue;
        }

        const int octaveup = segment->midi + (int)semitones_peroctave;

        if (
            segment->midi <
            nsk_options_program.profile.segments.denseframehighoctaveminmidi
        ) {
            continue;
        }

        struct _segment_templatefit current = {0};
        struct _segment_templatefit up = {0};
        int currentmidi[] = {
            segment->midi
        };
        int upmidi[] = {
            octaveup
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
            upmidi,
            NSK_SIZE(upmidi),
            &up
        )) {
            return false;
        }

        if (
            up.found &&
            up.score >=
            nsk_options_program.profile.segments.denseframehighoctaveminscore &&
            up.score - current.score >=
            nsk_options_program.profile.segments.denseframehighoctaveminimprovement
        ) {
            segment->midi = octaveup;
            segment->frequency = nsk_midi_tofreq(octaveup);
            segment->confidence = NSK_MAX(segment->confidence, up.score);
            segment->duty = up.duty;
            *changed = true;
        }
    }

    return true;
}
