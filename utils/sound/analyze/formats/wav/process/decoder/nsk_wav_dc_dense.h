#pragma once

#include <stdbool.h>

#include "nsk_wav_dc_marks.h"
#include "nsk_wav_dc_regions.h"
#include "nsk_wav_dc_sparse.h"
#include "../types/nsk_wav_ctx_meta.h"
#include "../../nsk_wav_type.h"

/*!
 * \brief  Calculates strict-mark density for dense-direct selection.
 *
 * \param[in] marks  Decoder mark evidence
 * \return Strict-mark ratio over the decoded frame range
 */
double nsk_wav_dc_dense_ratio(
    const struct nsk_wav_dc_marks *marks
);

/*!
 * \brief  Checks whether whole-file dense-direct decoding should run.
 *
 * \param[in] marks  Decoder mark evidence
 * \return True if strict-mark density enables dense-direct decoding
 */
bool nsk_wav_dc_dense_should_run(
    const struct nsk_wav_dc_marks *marks
);

/*!
 * \brief  Builds a whole-file dense-direct segment hypothesis.
 *
 * \param[in]      wav    Source WAV data
 * \param[in,out]  ctx    Processing context receiving segment hypothesis
 * \param[in]      marks  Decoder mark evidence
 * \return True if dense-direct decoding completed successfully
 */
bool nsk_wav_dc_dense_direct(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx *ctx,
    const struct nsk_wav_dc_marks *marks
);

/*!
 * \brief  Arbitrates between local-hybrid and all-dense segment hypotheses.
 *
 * \param[in]      wav        Source WAV data
 * \param[in,out]  ctx        Processing context receiving the chosen hypothesis
 * \param[in]      intervals  Snapped sparse decoder intervals
 * \param[in]      marks      Decoder mark evidence
 * \param[in]      regions    Local dense regions
 * \return True if arbitration completed successfully
 */
bool nsk_wav_dc_dense_arbitrate_local(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx *ctx,
    const struct nsk_wav_dc_intervals *intervals,
    const struct nsk_wav_dc_marks *marks,
    const struct nsk_wav_dc_regions *regions
);
