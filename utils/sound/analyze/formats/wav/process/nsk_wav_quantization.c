#include <math.h>
#include <stdlib.h>

#include <nsk_util_meta.h>

#include "nsk_wav_quantization.h"

#include "../../../arguments/nsk_args_options.h"

/*!
 * \brief  Adds one candidate to a quantized frame mark
 *
 * The mark stores pointers to already-created boundary candidates.  Candidate
 * ownership stays with \p ctx->candidates; the mark only groups candidates that
 * were close enough to the same grid frame.
 *
 * \param[in,out] ctx        Processing context
 * \param[in]     frame      Target frame index
 * \param[in]     candidate  Candidate assigned to the frame
 * \return        True if the candidate pointer was appended successfully
 */
static bool _mark_add(
    struct nsk_wav_ctx             *ctx,
    size_t                          frame,
    const struct nsk_wav_candidate *candidate
) {
    __typeof__(&ctx->marks.list[frame]) slot = &ctx->marks.list[frame];

    __typeof__(slot->candidates) ptr =
        realloc(slot->candidates, (slot->count + 1) * sizeof(*ptr));

    if (!ptr) {
        nsk_err(
            "Cannot reallocate the memory for the candidates list in mark"
        );
        return false;
    }

    slot->candidates = ptr;
    slot->candidates[slot->count++] = candidate;
    slot->maxstrength   = NSK_MAX(
        slot->maxstrength,
        candidate->strength
    );
    slot->maxabsdeltast = NSK_MAX(
        slot->maxabsdeltast,
        fabs(candidate->deltast)
    );

    slot->types[candidate->method]++;

    return true;
}

/*!
 * \brief  Creates mark pools based on the candidates
 *
 * Boundary candidates are continuous-time events.  This stage maps them onto
 * the target frame grid by rounding each candidate timestamp to the nearest
 * frame and checking that the timing error is within the configured window.
 *
 * The resulting mark array is dense: every internal frame has a mark slot, and
 * each slot contains the candidate pointers that landed near that frame.  The
 * later selector/decoder stages can then reason in integer frame coordinates
 * without losing the original candidate timestamps.
 *
 * \param[in]      wav  Source WAV data
 * \param[in,out]  ctx  Processing context receiving quantized marks
 * \return True if quantization completed successfully
 */
bool nsk_wav_quantization(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
) {
    const size_t framemax =
        ctx->samples.count *
        nsk_options_program.profile.boundary.grid.fps /
        wav->format.samplerate;

    ctx->marks.count = framemax;
    ctx->marks.list  = calloc(
        ctx->marks.count,
        sizeof(*ctx->marks.list)
    );
    if (!ctx->marks.list) {
        nsk_err(
            "Cannot allocate memory for the marks list"
        );
        return false;
    }

    for (size_t i = 0; i < framemax; i++) {
        ctx->marks.list[i].frame = i;
    }

    for (size_t i = 0; i < ctx->candidates.count; i++) {
        const struct nsk_wav_candidate *candidate =
            &ctx->candidates.candidate[i];

        const size_t frame = round(
            candidate->timestamp *
            nsk_options_program.profile.boundary.grid.fps
        );

        if (frame == 0 || frame >= framemax) {
            /* In the rare edge cases the position of the candidate, while being
             * still in the wav duration interval, could round up to the
             * framemax + 1:
             *
             * Assume wav.duration = 1.009 s
             * maxframe = floor(1.009 * 60fps) = 60
             * candidate.timestamp = 1.009
             * frame = 1.009 * 60 -> 60.54, which rounds up to 61 */
            continue;
        }

        const double frametime =
            (double)frame / nsk_options_program.profile.boundary.grid.fps;

        if (
            fabs(frametime - candidate->timestamp) >
            nsk_options_program.profile.quantization.windowerror
        ) {
            continue;
        }

        if (!_mark_add(
            ctx,
            frame,
            candidate
        )) {
            return false;
        }
    }

    return true;
}
