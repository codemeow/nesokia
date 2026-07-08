#pragma once

#include "../nsk_wav_sg_types.h"

/*!
 * \brief  Splits first frames in short low-note pulse-width cases.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[out]  changed  Set to true when the function modifies segment data.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_split_pulse_width_edgeframes(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    bool *changed
);
