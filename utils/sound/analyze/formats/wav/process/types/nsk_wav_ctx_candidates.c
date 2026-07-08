#include <stdlib.h>

#include <nsk_util_meta.h>

#include "nsk_wav_ctx_meta.h"

/*!
 * \brief  Appends a boundary candidate to the processing context
 *
 * \param[in,out] ctx        Processing context
 * \param[in]     candidate  Candidate to append
 * \return True if the candidate was appended successfully
 */
bool nsk_wav_ctx_candidate(
    struct nsk_wav_ctx      *ctx,
    struct nsk_wav_candidate candidate
) {
    __typeof__(ctx->candidates.candidate) ptr = realloc(
        ctx->candidates.candidate,
        sizeof(*ptr) * (ctx->candidates.count + 1)
    );
    if (!ptr) {
        nsk_err("Cannot realloc the candidates list");
        return false;
    }

    ctx->candidates.candidate = ptr;
    ctx->candidates.candidate[ctx->candidates.count++] = candidate;

    return true;
}
