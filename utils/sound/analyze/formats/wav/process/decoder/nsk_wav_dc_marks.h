#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "../types/nsk_wav_ctx_meta.h"
#include "../../nsk_wav_type.h"

/*!
 * \brief  Decoder boundary evidence mark.
 */
struct nsk_wav_dc_mark {
    size_t frame; /*!< Frame boundary carrying evidence */

    bool present;  /*!< Whether this frame has any boundary evidence */
    bool accepted; /*!< Whether selector logic accepted this boundary */
    bool strict;   /*!< Whether primary selector logic accepted this boundary */

    size_t strongcount; /*!< Number of strong period-like candidates */

    double p2pprevious; /*!< Normalized p2p value for the previous frame */
    double p2pcurrent;  /*!< Normalized p2p value for this frame */
};

/*!
 * \brief  Decoder mark storage with by-frame lookup arrays.
 */
struct nsk_wav_dc_marks {
    size_t count; /*!< Number of compact evidence marks */
    struct nsk_wav_dc_mark *list; /*!< Compact evidence mark list */

    size_t framecount; /*!< Number of addressable frame boundaries */
    bool *presentbyframe;  /*!< Evidence mark lookup by frame */
    bool *acceptedbyframe; /*!< Accepted mark lookup by frame */
    bool *strictbyframe;   /*!< Strict mark lookup by frame */
};

/*!
 * \brief  Releases memory owned by decoder marks.
 *
 * \param[in,out] marks  Decoder marks to clear
 */
void nsk_wav_dc_marks_free(struct nsk_wav_dc_marks *marks);

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
);
