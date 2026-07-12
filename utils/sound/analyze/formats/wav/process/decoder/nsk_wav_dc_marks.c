#include <math.h>
#include <stdlib.h>

#include <nsk_util_meta.h>

#include "nsk_wav_dc_marks.h"
#include "../segments/nsk_wav_sg_measure.h"
#include "../../../../arguments/nsk_args_options.h"

/*!
 * \brief  Counts strong period-like candidates for a mark.
 *
 * \param[in] mark  Quantized mark
 * \return Strong candidate count
 */
static size_t _dc_mark_strongcount(const struct nsk_wav_mark *mark) {
    return
        mark->types[NSK_WAV_CND_METHOD_MIXED_PERIOD_SOLVE] +
        mark->types[NSK_WAV_CND_METHOD_SHORT_PERIOD_EDGE];
}

/*!
 * \brief  Appends one decoder mark.
 *
 * \param[in,out] marks  Decoder mark storage
 * \param[in]     mark   Decoder mark to append
 * \return True if the mark was appended successfully
 */
static bool _dc_marks_append(
    struct nsk_wav_dc_marks *marks,
    struct nsk_wav_dc_mark mark
) {
    __typeof__(marks->list) ptr = realloc(
        marks->list,
        sizeof(*marks->list) * (marks->count + 1)
    );

    if (!ptr) {
        nsk_err("Cannot reallocate decoder marks list");
        return false;
    }

    marks->list = ptr;
    marks->list[marks->count++] = mark;

    return true;
}

/*!
 * \brief  Releases memory owned by decoder marks.
 *
 * \param[in,out] marks  Decoder marks to clear
 */
void nsk_wav_dc_marks_free(struct nsk_wav_dc_marks *marks) {
    if (!marks) {
        return;
    }

    free(marks->list);
    free(marks->presentbyframe);
    free(marks->acceptedbyframe);
    free(marks->strictbyframe);

    *marks = (struct nsk_wav_dc_marks) {0};
}

/*!
 * \brief  Builds decoder boundary evidence marks from quantized context marks.
 *
 * \param[in]      wav    Source WAV data
 * \param[in,out]  ctx    Processing context
 * \param[out]     marks  Decoder marks to fill
 * \return True if decoder marks were built successfully
 */
bool nsk_wav_dc_marks_build(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    struct nsk_wav_dc_marks *marks
) {
    *marks = (struct nsk_wav_dc_marks) {0};

    if (ctx->marks.count == 0) {
        return true;
    }

    marks->framecount = ctx->marks.count + 1;
    marks->presentbyframe = calloc(
        marks->framecount,
        sizeof(*marks->presentbyframe)
    );
    marks->acceptedbyframe = calloc(
        marks->framecount,
        sizeof(*marks->acceptedbyframe)
    );
    marks->strictbyframe = calloc(
        marks->framecount,
        sizeof(*marks->strictbyframe)
    );

    nsk_auto_free double *p2p = calloc(
        ctx->marks.count,
        sizeof(*p2p)
    );

    if (
        !marks->presentbyframe ||
        !marks->acceptedbyframe ||
        !marks->strictbyframe ||
        !p2p
    ) {
        nsk_wav_dc_marks_free(marks);
        nsk_err("Cannot allocate decoder mark lookup arrays");
        return false;
    }

    double p2pmax = 0.0;

    for (size_t frame = 0; frame < ctx->marks.count; frame++) {
        p2p[frame] = nsk_wav_sg_frame_p2p(wav, ctx, frame);
        p2pmax = NSK_MAX(p2pmax, p2p[frame]);
    }

    if (p2pmax > 0.0) {
        for (size_t frame = 0; frame < ctx->marks.count; frame++) {
            p2p[frame] /= p2pmax;
        }
    }

    for (size_t frame = 0; frame < ctx->marks.count; frame++) {
        const struct nsk_wav_mark *mark = &ctx->marks.list[frame];
        const double previous = frame > 0 ? p2p[frame - 1] : 0.0;
        const double current = p2p[frame];

        const bool p2ponset =
            previous <
                nsk_options_program.profile.decoder.markp2pactivethreshold &&
            current >=
                nsk_options_program.profile.decoder.markp2pactivethreshold;
        const bool p2pdelta =
            fabs(current - previous) >=
                nsk_options_program.profile.decoder.markp2pdeltathreshold &&
            current >=
                nsk_options_program.profile.decoder.markp2pactivethreshold;

        const bool present =
            mark->count > 0 ||
            mark->accepted ||
            p2ponset ||
            p2pdelta;

        if (!present) {
            continue;
        }

        marks->presentbyframe[frame] = true;
        marks->acceptedbyframe[frame] = mark->accepted;
        marks->strictbyframe[frame] = mark->accepted;

        if (!_dc_marks_append(
            marks,
            (struct nsk_wav_dc_mark) {
                .frame = frame,
                .present = true,
                .accepted = mark->accepted,
                .strict = mark->accepted,
                .strongcount = _dc_mark_strongcount(mark),
                .p2pprevious = previous,
                .p2pcurrent = current
            }
        )) {
            nsk_wav_dc_marks_free(marks);
            return false;
        }

    }

    return true;
}
