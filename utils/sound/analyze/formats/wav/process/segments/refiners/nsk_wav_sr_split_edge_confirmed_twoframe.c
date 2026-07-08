#include "nsk_wav_sr_split_edge_confirmed_twoframe.h"
#include "../nsk_wav_sg_edgeperiod.h"
#include "../nsk_wav_sg_list.h"
#include "../nsk_wav_sg_pulsewidth.h"
#include "../nsk_wav_sg_classify.h"
#include "../nsk_wav_sg_template.h"

/*!
 * \brief  Splits exact two-frame segments when edge-period evidence confirms the split.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[out]  changed  Set to true when the function modifies segment data.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_split_edge_confirmed_twoframe(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    bool *changed
) {
    *changed = false;

    for (size_t i = 0; i < ctx->segments.count; i++) {
        const struct nsk_wav_segment segment = ctx->segments.list[i];

        if (
            !segment.active ||
            segment.frameend - segment.framestart != 2
        ) {
            continue;
        }

        if (
            fabs(segment.duty - 12.5) <=
            nsk_options_program.profile.segments
                .mergedutyequalitythreshold &&
            segment.confidence >=
            nsk_options_program.profile.segments
                .framegridsplitstrongconfidence
        ) {
            continue;
        }

        const size_t splitframe = segment.framestart + 1;
        struct nsk_wav_segment left = nsk_wav_sg_fromframes(
            wav, ctx,
            segment.framestart,
            splitframe
        );
        struct nsk_wav_segment right = nsk_wav_sg_fromframes(
            wav, ctx,
            splitframe,
            segment.frameend
        );

        if (!nsk_wav_sg_recalc(wav, ctx, &left)) {
            return false;
        }

        if (!nsk_wav_sg_recalc(wav, ctx, &right)) {
            return false;
        }

        struct _segment_pitch edge = {0};

        if (!nsk_wav_sg_edgeperiod(
            wav, ctx,
            right.samplestart,
            right.sampleend,
            &edge
        )) {
            return false;
        }

        bool use =
            left.active &&
            right.active &&
            left.midi == segment.midi &&
            right.midi >= 0 &&
            right.midi != segment.midi &&
            edge.found &&
            edge.midi == right.midi &&
            edge.confidence >=
            nsk_options_program.profile.segments.edgeconfirmedsplitminconfidence &&
            edge.support >=
            nsk_options_program.profile.segments.edgeconfirmedsplitminsupport &&
            left.confidence >=
            nsk_options_program.profile.segments.edgeconfirmedsplitminstateconfidence &&
            right.confidence >=
            nsk_options_program.profile.segments.edgeconfirmedsplitminstateconfidence;

        if (!use && segment.midi <= nsk_options_program.profile.segments.pulsewidthsplitmaxmidi) {
            struct _segment_pitch spanedge = {0};

            if (!nsk_wav_sg_edgeperiod(
                wav, ctx,
                segment.samplestart,
                segment.sampleend,
                &spanedge
            )) {
                return false;
            }

            struct _segment_pulsewidth pulse = {0};

            if (!nsk_wav_sg_pulse_width_note_oneframe(
                wav, ctx,
                segment.framestart,
                &pulse
            )) {
                return false;
            }

            struct nsk_wav_segment fallbackright = right;
            bool ignored = false;

            if (!nsk_wav_sg_segment_template_override(wav, ctx, &fallbackright, &ignored)) {
                return false;
            }

            if (
                spanedge.found &&
                spanedge.midi != segment.midi &&
                spanedge.confidence >=
                nsk_options_program.profile.segments
                    .edgeconfirmedsplitminconfidence &&
                spanedge.support >=
                nsk_options_program.profile.segments
                    .edgeconfirmedsplitminsupport &&
                pulse.found &&
                pulse.midi == spanedge.midi &&
                pulse.templatescore >=
                nsk_options_program.profile.segments
                    .pulsewidthsplitmintemplatescore &&
                pulse.templatemargin >=
                nsk_options_program.profile.segments
                    .denseframepulsemargin &&
                fallbackright.active &&
                fallbackright.midi == segment.midi &&
                fallbackright.confidence >=
                nsk_options_program.profile.segments
                    .edgeconfirmedsplitminstateconfidence
            ) {
                left.active = true;
                left.midi = pulse.midi;
                left.frequency = nsk_midi_tofreq(pulse.midi);
                left.confidence = NSK_MAX(
                    left.confidence,
                    NSK_MAX(pulse.confidence, pulse.templatescore)
                );
                right = fallbackright;
                use = true;
            }
        }

        if (!use) {
            continue;
        }

        const struct nsk_wav_segment items[] = {
            left,
            right
        };

        if (!nsk_wav_sg_replace_range(wav, ctx, i, 1, items, NSK_SIZE(items))) {
            return false;
        }

        *changed = true;
    }

    return true;
}
