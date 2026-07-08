#include "nsk_wav_sr_split_confident_frame_grid.h"
#include "../nsk_wav_sg_state.h"
#include "../nsk_wav_sg_list.h"
#include "../nsk_wav_sg_pulsewidth.h"
#include "../nsk_wav_sg_classify.h"
#include "../nsk_wav_sg_template.h"

/*!
 * \brief  Splits short dense segments into confident per-frame groups.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[out]  changed  Set to true when the function modifies segment data.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_split_confident_frame_grid(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    bool *changed
) {
    *changed = false;

    if (
        nsk_wav_sg_dense_ratio(wav, ctx) <
        nsk_options_program.profile.segments.framegridsplitmindensesratio
    ) {
        return true;
    }

    for (size_t i = 0; i < ctx->segments.count; i++) {
        const struct nsk_wav_segment segment = ctx->segments.list[i];
        const size_t framecount = segment.frameend - segment.framestart;

        if (
            !segment.active ||
            framecount < 2 ||
            framecount >
            nsk_options_program.profile.segments.framegridsplitmaxframes ||
            segment.framestart == 0 ||
            segment.frameend + 1 >= ctx->marks.count
        ) {
            continue;
        }

        struct nsk_wav_segment frames[framecount];
        bool valid = true;

        for (size_t fi = 0; fi < framecount; fi++) {
            frames[fi] = nsk_wav_sg_fromframes(
                wav, ctx,
                segment.framestart + fi,
                segment.framestart + fi + 1
            );

            if (!nsk_wav_sg_recalc(wav, ctx, &frames[fi])) {
                return false;
            }

            bool ignored = false;

            if (!nsk_wav_sg_segment_template_override(wav, ctx, &frames[fi], &ignored)) {
                return false;
            }

            struct _segment_pulsewidth pulse = {0};

            if (!nsk_wav_sg_pulse_width_note_oneframe(
                wav, ctx,
                frames[fi].framestart,
                &pulse
            )) {
                return false;
            }

            if (
                pulse.found &&
                pulse.midi <=
                nsk_options_program.profile.segments.framegridsplitpulsemaxmidi &&
                pulse.templatescore >=
                nsk_options_program.profile.segments.framegridsplitpulseminscore &&
                pulse.templatemargin >=
                nsk_options_program.profile.segments.framegridsplitpulsemargin &&
                (
                    frames[fi].midi < 0 ||
                    pulse.confidence >=
                    frames[fi].confidence *
                    nsk_options_program.profile.segments
                        .framegridsplitpulseconfidenceratio ||
                    pulse.templatescore >=
                    frames[fi].confidence *
                    nsk_options_program.profile.segments
                        .framegridsplitpulseconfidenceratio ||
                    (size_t)abs(pulse.midi - frames[fi].midi) <=
                    nsk_options_program.profile.segments
                        .framegridsplitpulsemaxdelta
                )
            ) {
                frames[fi].active = true;
                frames[fi].midi = pulse.midi;
                frames[fi].frequency = nsk_midi_tofreq(pulse.midi);
                frames[fi].confidence = NSK_MAX(
                    frames[fi].confidence,
                    NSK_MAX(pulse.confidence, pulse.templatescore)
                );
                frames[fi].duty = pulse.duty;
            }

            if (
                !frames[fi].active ||
                frames[fi].midi < 0 ||
                frames[fi].confidence <
                nsk_options_program.profile.segments.framegridsplitminconfidence
            ) {
                valid = false;
                break;
            }
        }

        if (!valid) {
            continue;
        }

        struct nsk_wav_segment groups[framecount];
        size_t groupcount = 0;
        bool originalseen = false;

        for (size_t fi = 0; fi < framecount; fi++) {
            if (frames[fi].midi == segment.midi) {
                originalseen = true;
            }

            if (
                groupcount > 0 &&
                nsk_wav_sg_isequal(&groups[groupcount - 1], &frames[fi])
            ) {
                nsk_wav_sg_mergeitem(&groups[groupcount - 1], &frames[fi]);
                continue;
            }

            groups[groupcount++] = frames[fi];
        }

        if (
            groupcount <= 1 ||
            (
                !originalseen &&
                segment.confidence >=
                nsk_options_program.profile.segments.framegridsplitstrongconfidence
            )
        ) {
            continue;
        }

        if (!nsk_wav_sg_replace_range(wav, ctx, i, 1, groups, groupcount)) {
            return false;
        }

        *changed = true;
    }

    return true;
}
