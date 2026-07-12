#include <stdlib.h>

#include <nsk_util_meta.h>

#include "nsk_wav_ctx_meta.h"

/*!
 * \brief  Clears decoded event storage in the processing context
 *
 * \param[in,out] ctx  Processing context
 */
void nsk_wav_ctx_notes_clear(
    struct nsk_wav_ctx *ctx
) {
    if (!ctx) {
        return;
    }

    free(ctx->notes.list);
    ctx->notes.list = NULL;
    ctx->notes.count = 0;
}

/*!
 * \brief  Appends a decoded event to the processing context
 *
 * \param[in,out] ctx   Processing context
 * \param[in]     note  Decoded event to append
 * \return True if the event was appended successfully
 */
bool nsk_wav_ctx_note(
    struct nsk_wav_ctx *ctx,
    struct nsk_wav_note note
) {
    __typeof__(ctx->notes.list) ptr = realloc(
        ctx->notes.list,
        sizeof(*ptr) * (ctx->notes.count + 1)
    );
    if (!ptr) {
        nsk_err("Cannot realloc the decoded events list");
        return false;
    }

    ctx->notes.list = ptr;
    ctx->notes.list[ctx->notes.count++] = note;

    return true;
}
