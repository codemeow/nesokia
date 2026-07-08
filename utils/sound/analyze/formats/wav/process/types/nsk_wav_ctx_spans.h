#pragma once

#include <stdbool.h>
#include <stddef.h>

/*!
 * \brief  WAV energy span
 */
struct nsk_wav_span {
    size_t start; /*!< Start of the span in sample index */
    size_t end;   /*!< End of the span in sample index */
};

/*!
 * \brief  Energy span storage owned by the processing context
 */
struct nsk_wav_ctx_spans {
    size_t count;              /*!< Number of spans */
    struct nsk_wav_span *span; /*!< Array of detected spans */
};

struct nsk_wav_ctx;

/*!
 * \brief  Appends an energy span to the processing context
 *
 * \param[in,out] ctx   Processing context
 * \param[in]     span  Span to append
 * \return True if the span was appended successfully
 */
__attribute__((warn_unused_result))
bool nsk_wav_ctx_span(
    struct nsk_wav_ctx *ctx,
    struct nsk_wav_span span
);
