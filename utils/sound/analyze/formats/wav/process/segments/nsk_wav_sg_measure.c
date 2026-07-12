#include "nsk_wav_sg_measure.h"

/*!
 * \brief  Calculates peak-to-peak amplitude for a single frame.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  frame  Frame index or frame position.
 * \return Calculated floating-point value.
 */
double nsk_wav_sg_frame_p2p(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    size_t frame
) {
    const double fps = nsk_options_program.profile.boundary.grid.fps;
    const double samplerate = wav->format.samplerate;

    size_t start = lround((double)frame * samplerate / fps);
    size_t end   = lround((double)(frame + 1) * samplerate / fps);

    double min = ctx->samples.value[start];
    double max = ctx->samples.value[start];

    for (size_t i = start + 1; i < end; i++) {
        min = NSK_MIN(min, ctx->samples.value[i]);
        max = NSK_MAX(max, ctx->samples.value[i]);
    }

    return max - min;
}

/*!
 * \brief  Calculates the global silence threshold from frame peak-to-peak values.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_calcsilence(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
) {
    nsk_auto_free double *nonzero = calloc(
        ctx->marks.count,
        sizeof(*nonzero)
    );
    if (!nonzero) {
        nsk_err("Cannot allocate memory for frame p2p values");
        return false;
    }

    size_t count = 0;

    for (size_t frame = 0; frame < ctx->marks.count; frame++) {
        const double p2p = nsk_wav_sg_frame_p2p(wav, ctx, frame);

        if (p2p > nsk_options_program.profile.segments.silencep2pfloor) {
            nonzero[count++] = p2p;
        }
    }

    if (count == 0) {
        ctx->segments.silence =
            nsk_options_program.profile.segments.silencefallback;
        return true;
    }

    double quantile = 0.0;
    if (!nsk_math_quantiled(
        nsk_options_program.profile.segments.silencequantile,
        nonzero,
        count,
        &quantile
    )) {
        return false;
    }

    const double threshold =
        quantile * nsk_options_program.profile.segments.silencescale;

    ctx->segments.silence = nsk_math_clampd(
        threshold,
        nsk_options_program.profile.segments.silencemin,
        nsk_options_program.profile.segments.silencemax
    );

    return true;
}

/*!
 * \brief  Calculates peak-to-peak amplitude for a segment.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in,out]  segment  Segment being analyzed or updated.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_p2p(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    struct nsk_wav_segment *segment
) {
    (void)wav;

    double samplemin = ctx->samples.value[segment->samplestart];
    double samplemax = samplemin;

    for (size_t i = segment->samplestart + 1; i < segment->sampleend; i++) {
        samplemin = NSK_MIN(samplemin, ctx->samples.value[i]);
        samplemax = NSK_MAX(samplemax, ctx->samples.value[i]);
    }

    segment->p2p  = samplemax - samplemin;
    return true;
}

/*!
 * \brief  Calculates normalized volume for a segment.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in,out]  segment  Segment being analyzed or updated.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_volume(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    struct nsk_wav_segment *segment
) {
    (void)wav;
    (void)ctx;

    if (!segment->active) {
        return true;
    }

    segment->volume = round(
        nsk_math_clampd(
            segment->p2p / 2,
            0.0,
            1.0
        ) * 100
    );
    return true;
}

/*!
 * \brief  Classifies a segment as active sound or rest.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in,out]  segment  Segment being analyzed or updated.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_active(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    struct nsk_wav_segment *segment
) {
    (void)wav;

    segment->active = segment->p2p >= ctx->segments.silence;
    return true;
}

/*!
 * \brief  Calculates peak-to-peak amplitude delta around a frame boundary.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  frame  Frame index or frame position.
 * \return Calculated floating-point value.
 */
double nsk_wav_sg_mark_p2p_delta(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    size_t frame
) {
    if (frame == 0 || frame >= ctx->marks.count) {
        return DBL_MAX;
    }

    if (
        ctx->marks.list[frame].count == 0 &&
        !ctx->marks.list[frame].accepted &&
        !ctx->marks.list[frame].strict
    ) {
        return DBL_MAX;
    }

    return fabs(
        nsk_wav_sg_frame_p2p(wav, ctx, frame) -
        nsk_wav_sg_frame_p2p(wav, ctx, frame - 1)
    );
}

/*!
 * \brief  Estimates folded pulse duty from the full segment waveform.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in,out]  segment  Segment being analyzed or updated.
 * \param[in]  duty  Pulse duty candidate.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_estimate_folded_duty(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    const struct nsk_wav_segment *segment,
    double *duty
) {
    (void)wav;

    *duty = 0.0;

    if (segment->sampleend - segment->samplestart < 4) {
        return false;
    }

    double peak = ctx->samples.value[segment->samplestart];
    double trough = peak;

    for (size_t i = segment->samplestart + 1; i < segment->sampleend; i++) {
        peak = NSK_MAX(peak, ctx->samples.value[i]);
        trough = NSK_MIN(trough, ctx->samples.value[i]);
    }

    if (peak - trough <= nsk_options_program.profile.segments.edgeperiodp2pfloor) {
        return false;
    }

    const double midline = (peak + trough) * 0.5;
    size_t highcount = 0;
    const size_t samplecount = segment->sampleend - segment->samplestart;

    for (size_t i = segment->samplestart; i < segment->sampleend; i++) {
        if (ctx->samples.value[i] > midline) {
            highcount++;
        }
    }

    const double highfraction = (double)highcount / samplecount;
    const double folded =
        NSK_MIN(highfraction, 1.0 - highfraction) *
        100.0;

    if (folded <= nsk_options_program.profile.segments.foldedduty12max) {
        *duty = 12.5;
        return true;
    }

    if (
        folded >= nsk_options_program.profile.segments.foldedduty25min &&
        folded <= nsk_options_program.profile.segments.foldedduty25max
    ) {
        *duty = 25.0;
        return true;
    }

    if (folded >= nsk_options_program.profile.segments.foldedduty50min) {
        *duty = 50.0;
        return true;
    }

    return false;
}
