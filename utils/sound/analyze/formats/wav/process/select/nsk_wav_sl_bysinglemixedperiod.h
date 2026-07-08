#pragma once

#include <stdbool.h>

#include "../types/nsk_wav_ctx_marks.h"

/*!
 * \brief  Selects a mark by one high-quality mixed-period candidate.
 *
 * \param[in] mark  Quantized mark
 * \return True if the mark passes single-mixed rescue thresholds
 */
bool nsk_wav_sl_bysinglemixedperiod(
    const struct nsk_wav_mark *mark
);
