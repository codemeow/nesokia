#pragma once

#include "nsk_wav_sg_types.h"

/*!
 * \brief  Estimates a one-frame low note from opposite-polarity pulse width.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  frame  Frame index or frame position.
 * \param[out]  result  Calculated result.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_pulse_width_note_oneframe(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    size_t frame,
    struct _segment_pulsewidth *result
);
