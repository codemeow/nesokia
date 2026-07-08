#include <stdlib.h>

#include "nsk_wav_ctx_meta.h"

/*!
 * \brief  Releases all allocations owned by a processing context
 *
 * \param[in,out] ctx  Processing context
 */
void nsk_wav_ctx_free(struct nsk_wav_ctx *ctx) {
    if (!ctx) {
        return;
    }

    free(ctx->samples.value);
    free(ctx->candidates.candidate);
    free(ctx->spans.span);
    free(ctx->edges.rise.edge);
    free(ctx->edges.fall.edge);
    for (size_t i = 0; i < ctx->marks.count; i++) {
        free(ctx->marks.list[i].candidates);
    }
    free(ctx->marks.list);
    free(ctx->segments.list);
}
