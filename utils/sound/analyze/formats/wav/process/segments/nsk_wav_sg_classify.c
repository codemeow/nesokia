#include "nsk_wav_sg_classify.h"
#include "nsk_wav_sg_measure.h"
#include "nsk_wav_sg_spectral.h"

/*!
 * \brief  Calculates the trimmed sample bounds used for segment classification.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in,out]  segment  Segment being analyzed or updated.
 * \param[in]  samplestart  Inclusive sample start.
 * \param[in]  sampleend  Exclusive sample end.
 */
void nsk_wav_sg_corebounds(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    const struct nsk_wav_segment *segment,
    size_t *samplestart,
    size_t *sampleend
) {
    (void)ctx;

    const uint32_t samplerate = wav->format.samplerate;
    const double fps = nsk_options_program.profile.boundary.grid.fps;
    const size_t framecount = segment->frameend - segment->framestart;

    double coreframestart = segment->framestart;
    double coreframeend   = segment->frameend;

    const size_t marginframes =
        nsk_options_program.profile.segments.marginframes;

    if (
        marginframes > 0 &&
        framecount >= marginframes * 2 + 1
    ) {
        coreframestart += marginframes;
        coreframeend   -= marginframes;
    }

    if (framecount > nsk_options_program.profile.segments.notrimshortframes) {
        const double duration =
            (coreframeend - coreframestart) /
            fps;
        const double ratiotrim =
            duration *
            NSK_MAX(0.0, nsk_options_program.profile.segments.trimratio);
        const double trim = NSK_MIN(
            NSK_MAX(0.0, nsk_options_program.profile.segments.trim),
            ratiotrim
        );
        const double core = duration - trim * 2.0;

        if (
            trim > 0.0 &&
            core >= nsk_options_program.profile.segments.mincore
        ) {
            const double trimframes =
                trim *
                fps;

            coreframestart += trimframes;
            coreframeend   -= trimframes;
        }
    }

    *samplestart = lround(coreframestart * samplerate / fps);
    *sampleend =
        lround(coreframeend * samplerate / fps);
}

/*!
 * \brief  Classifies a segment pitch and duty from its spectral evidence.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in,out]  segment  Segment being analyzed or updated.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_classify(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    struct nsk_wav_segment *segment
) {
    if (!segment->active) {
        segment->midi = -1;
        return true;
    }

    size_t coresamplestart = 0;
    size_t coresampleend = 0;

    nsk_wav_sg_corebounds(
        wav, ctx,
        segment,
        &coresamplestart,
        &coresampleend
    );

    struct _segment_pitch pitch = {0};

    if (!nsk_wav_sg_analyze_window(
        wav, ctx,
        coresamplestart,
        coresampleend,
        segment->frameend - segment->framestart,
        &pitch
    )) {
        return false;
    }

    segment->midi          = pitch.midi;
    segment->frequency     = pitch.found ? pitch.frequency : 0.0;
    segment->confidence    = pitch.confidence;
    segment->harmonicratio = pitch.ratio;
    segment->duty          = pitch.duty;

    return true;
}

/*!
 * \brief  Recomputes all derived measurements and classification for a segment.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in,out]  segment  Segment being analyzed or updated.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_recalc(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    struct nsk_wav_segment *segment
) {
    if (!nsk_wav_sg_p2p(wav, ctx, segment)) {
        return false;
    }

    if (!nsk_wav_sg_active(wav, ctx, segment)) {
        return false;
    }

    if (!nsk_wav_sg_volume(wav, ctx, segment)) {
        return false;
    }

    if (!nsk_wav_sg_classify(wav, ctx, segment)) {
        return false;
    }

    return true;
}
