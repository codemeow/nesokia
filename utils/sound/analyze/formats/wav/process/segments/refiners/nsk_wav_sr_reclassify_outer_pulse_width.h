#pragma once

#include "../nsk_wav_sg_types.h"

/*!
 * \brief  Reclassifies short outer segments using pulse-width and template evidence.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[out]  changed  Set to true when the function modifies segment data.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_reclassify_outer_pulse_width(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    bool *changed
);
