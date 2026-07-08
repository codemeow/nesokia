#pragma once

#include <stdbool.h>

#include "../types/nsk_wav_ctx_marks.h"

/*!
 * \brief  Selects a mark by real period evidence plus transition probes.
 *
 * \param[in] mark  Quantized mark
 * \return True if the mark passes transition rescue thresholds
 */
bool nsk_wav_sl_byperiodtransition(
    const struct nsk_wav_mark *mark
);
