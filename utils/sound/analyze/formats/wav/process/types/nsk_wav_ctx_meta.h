#pragma once

#include <stdbool.h>

#include "nsk_wav_ctx_candidates.h"
#include "nsk_wav_ctx_edges.h"
#include "nsk_wav_ctx_marks.h"
#include "nsk_wav_ctx_samples.h"
#include "nsk_wav_ctx_segments.h"
#include "nsk_wav_ctx_spans.h"

/*!
 * \brief  Processing state derived from an immutable WAV input
 */
struct nsk_wav_ctx {
    struct nsk_wav_ctx_samples    samples;    /*!< Working samples */
    struct nsk_wav_ctx_candidates candidates; /*!< Boundary candidates */
    struct nsk_wav_ctx_spans      spans;      /*!< Energy spans */
    struct nsk_wav_ctx_edges      edges;      /*!< Edge lists */
    struct nsk_wav_ctx_marks      marks;      /*!< Quantized marks */
    struct nsk_wav_ctx_segments   segments;   /*!< Classified segments */
};

/*!
 * \brief  Releases all allocations owned by a processing context
 *
 * \param[in,out] ctx  Processing context
 */
void nsk_wav_ctx_free(struct nsk_wav_ctx *ctx);
