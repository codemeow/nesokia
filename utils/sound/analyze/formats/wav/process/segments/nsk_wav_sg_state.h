#pragma once

#include "nsk_wav_sg_types.h"

/*!
 * \brief  Calculates the accepted dense-mark ratio.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \return Calculated floating-point value.
 */
double nsk_wav_sg_dense_ratio(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
);

/*!
 * \brief  Calculates the accepted-mark ratio.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \return Calculated floating-point value.
 */
double nsk_wav_sg_accepted_ratio(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
);

/*!
 * \brief  Checks whether the mark set is dense enough for direct dense processing.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_isdensedirect(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
);
