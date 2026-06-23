#include <math.h>
#include <stdlib.h>

#include <nsk_util_meta.h>

#include "nsk_wav_quantization.h"

#include "../../../arguments/nsk_args_options.h"

/*!
 * \brief  Adds one candidate to a quantized frame mark
 *
 * The mark stores pointers to already-created boundary candidates.  Candidate
 * ownership stays with \p wav->candidates; the mark only groups candidates that
 * were close enough to the same grid frame.
 *
 * \param[in,out] wav        The wav
 * \param[in]     frame      Target frame index
 * \param[in]     candidate  Candidate assigned to the frame
 * \return        True if the candidate pointer was appended successfully
 */
static bool _mark_add(
    struct nsk_wav                 *wav,
    size_t                          frame,
    const struct nsk_wav_candidate *candidate
) {
    __typeof__(&wav->marks.list[frame]) slot = &wav->marks.list[frame];

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
    return true;
}

/*!
 * \brief  Logs the parameters of the time marks
 *
 * \param[in] wav  The wav
 */
static void _mark_log(
    const struct nsk_wav *wav
) {
    size_t filled = 0;
    for (size_t i = 0; i < wav->marks.count; i++) {
        if (wav->marks.list[i].count > 0) {
            filled++;
        }
    }

    nsk_inf("    - (+%zu marks)\n", filled);
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
 * \param[in,out]  wav   The wav
 * \return True if quantization completed successfully
 */
bool nsk_wav_quantization(
    struct nsk_wav      *wav
) {
    const size_t framemax =
        wav->samples.raw.count *
        nsk_options_program.profile.boundary.grid.fps /
        wav->format.samplerate;

    wav->marks.count = framemax;
    wav->marks.list  = calloc(
        wav->marks.count,
        sizeof(*wav->marks.list)
    );
    if (!wav->marks.list) {
        nsk_err(
            "Cannot allocate memory for the marks list"
        );
        return false;
    }

    for (size_t i = 0; i < framemax; i++) {
        wav->marks.list[i].frame = i;
    }

    for (size_t i = 0; i < wav->candidates.count; i++) {
        const struct nsk_wav_candidate *candidate =
            &wav->candidates.candidate[i];

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
            wav,
            frame,
            candidate
        )) {
            return false;
        }
    }

    _mark_log(wav);

    return true;
}
