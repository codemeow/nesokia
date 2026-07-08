#include <stdlib.h>

#include <nsk_util_meta.h>

#include "nsk_wav_ctx_meta.h"

/*!
 * \brief  Appends an energy span to the processing context
 *
 * \param[in,out] ctx   Processing context
 * \param[in]     span  Span to append
 * \return True if the span was appended successfully
 */
bool nsk_wav_ctx_span(
    struct nsk_wav_ctx *ctx,
    struct nsk_wav_span span
) {
    __typeof__(ctx->spans.span) ptr = realloc(
        ctx->spans.span,
        sizeof(*ptr) * (ctx->spans.count + 1)
    );
    if (!ptr) {
        nsk_err("Cannot realloc the span list");
        return false;
    }

    ctx->spans.span = ptr;
    ctx->spans.span[ctx->spans.count++] = span;

    return true;
}
