#include "nsk_wav_sg_pulsewidth.h"
#include "nsk_wav_sg_list.h"
#include "nsk_wav_sg_template.h"

/*!
 * \brief  Estimates a one-frame low note from opposite-polarity pulse width.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  frame  Frame index or frame position.
 * \param[out]  result  Calculated result.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_pulse_width_note_oneframe(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    size_t frame,
    struct _segment_pulsewidth *result
) {
    *result = (struct _segment_pulsewidth) {
        .found = false,
        .midi  = -1,
        .templatescore = -1.0
    };

    const size_t samplestart = nsk_wav_sg_frame_sample(wav, ctx, frame);
    const size_t sampleend = nsk_wav_sg_frame_sample(wav, ctx, frame + 1);
    const size_t samplecount = sampleend - samplestart;
    const double freqlowest =
        nsk_options_program.profile.boundary.edgetrain.frequencylowest;
    const double freqhighest =
        nsk_options_program.profile.boundary.edgetrain.frequencyhighest;

    if (samplecount < nsk_options_program.profile.segments.edgeperiodminsamples) {
        return true;
    }

    const size_t window = NSK_MAX(
        (size_t)3,
        (size_t)lround(
            (double)wav->format.samplerate *
            nsk_options_program.profile.boundary.schmitt.window
        )
    );
    const size_t leftpad = window / 2;

    nsk_auto_free double *rollingmax = calloc(samplecount, sizeof(*rollingmax));
    nsk_auto_free double *rollingmin = calloc(samplecount, sizeof(*rollingmin));

    if (!rollingmax || !rollingmin) {
        nsk_err("Cannot allocate memory for pulse-width rolling Schmitt data");
        return false;
    }

    for (size_t i = 0; i < samplecount; i++) {
        double max = ctx->samples.value[samplestart];
        double min = max;

        for (size_t wi = 0; wi < window; wi++) {
            const size_t padded = i + wi;
            size_t source = 0;

            if (padded < leftpad) {
                source = 0;

            } else if (padded >= leftpad + samplecount) {
                source = samplecount - 1;

            } else {
                source = padded - leftpad;
            }

            const double sample = ctx->samples.value[samplestart + source];

            max = NSK_MAX(max, sample);
            min = NSK_MIN(min, sample);
        }

        rollingmax[i] = max;
        rollingmin[i] = min;
    }

    if (samplecount == 0) {
        return true;
    }

    struct {
        double time;
        int kind;
    } events[samplecount];
    size_t eventcount = 0;
    const double firstmidline = (rollingmax[0] + rollingmin[0]) * 0.5;
    int state = ctx->samples.value[samplestart] > firstmidline ? 1 : -1;

    for (size_t i = 1; i < samplecount; i++) {
        const double previous = ctx->samples.value[samplestart + i - 1];
        const double current = ctx->samples.value[samplestart + i];
        const double midline = (rollingmax[i] + rollingmin[i]) * 0.5;
        const double amplitude = NSK_MAX(
            (rollingmax[i] - rollingmin[i]) * 0.5,
            1e-9
        );
        const double highthreshold =
            midline +
            amplitude *
            nsk_options_program.profile.boundary.schmitt.hysteresis;
        const double lowthreshold =
            midline -
            amplitude *
            nsk_options_program.profile.boundary.schmitt.hysteresis;
        int kind = 0;

        if (state <= 0 && current > highthreshold) {
            kind = 1;
            state = 1;

        } else if (state >= 0 && current < lowthreshold) {
            kind = -1;
            state = -1;
        }

        if (kind != 0) {
            const double denominator = current - previous;
            double fraction =
                denominator == 0.0 ?
                0.0 :
                (midline - previous) / denominator;

            fraction = nsk_math_clampd(fraction, 0.0, 1.0);

            events[eventcount++] = (__typeof__(events[0])) {
                .time = ((double)i - 1.0 + fraction) /
                    wav->format.samplerate,
                .kind = kind
            };
        }
    }

    if (eventcount < 2) {
        return true;
    }

    struct {
        bool found;
        int midi;
        double edgescore;
        double duty;
    } candidates[eventcount * NSK_SIZE(_segment_duties)];
    size_t candidatecount = 0;

    for (size_t i = 1; i < eventcount; i++) {
        if (events[i - 1].kind == events[i].kind) {
            continue;
        }

        const double width = events[i].time - events[i - 1].time;

        if (width <= 0.0) {
            continue;
        }

        for (size_t di = 0; di < NSK_SIZE(_segment_duties); di++) {
            const double duty = _segment_duties[di];
            const double fraction =
                events[i - 1].kind > 0 ?
                duty / 100.0 :
                1.0 - duty / 100.0;

            if (fraction <= 0.0) {
                continue;
            }

            const double frequency = fraction / width;
            const int midi = nsk_midi_fromfreq(frequency);
            const double expected = nsk_midi_tofreq(midi);

            if (expected < freqlowest || expected > freqhighest) {
                continue;
            }

            const double cents = fabs(1200.0 * log2(frequency / expected));
            const double edgescore = NSK_MAX(
                0.0,
                1.0 -
                cents /
                nsk_options_program.profile.segments.pulsewidthcentsscale
            );
            bool seen = false;

            for (size_t ci = 0; ci < candidatecount; ci++) {
                if (candidates[ci].midi != midi) {
                    continue;
                }

                seen = true;

                if (edgescore > candidates[ci].edgescore) {
                    candidates[ci].edgescore = edgescore;
                    candidates[ci].duty = duty;
                }

                break;
            }

            if (!seen) {
                candidates[candidatecount++] = (__typeof__(candidates[0])) {
                    .found = true,
                    .midi = midi,
                    .edgescore = edgescore,
                    .duty = duty
                };
            }
        }
    }

    double bestcombined = -1.0;
    double secondcombined = 0.0;
    struct _segment_pulsewidth best = *result;

    for (size_t ci = 0; ci < candidatecount; ci++) {
        struct _segment_templatefit fit = {0};

        if (!nsk_wav_sg_template_fit(
            wav, ctx,
            samplestart,
            sampleend,
            candidates[ci].midi,
            candidates[ci].duty,
            &fit
        )) {
            return false;
        }

        const double combined =
            candidates[ci].edgescore *
            NSK_MAX(0.0, fit.score);

        if (combined > bestcombined) {
            secondcombined = bestcombined > 0.0 ? bestcombined : 0.0;
            bestcombined = combined;
            best = (struct _segment_pulsewidth) {
                .found = true,
                .midi = candidates[ci].midi,
                .confidence = combined,
                .support = 1,
                .duty = candidates[ci].duty,
                .templatescore = fit.score,
                .templatemargin = combined - secondcombined
            };

        } else if (combined > secondcombined) {
            secondcombined = combined;
            best.templatemargin = bestcombined - secondcombined;
        }
    }

    if (best.found) {
        best.templatemargin = bestcombined - secondcombined;
        *result = best;
    }

    return true;
}
