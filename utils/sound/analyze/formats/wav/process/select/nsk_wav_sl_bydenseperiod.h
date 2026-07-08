#pragma once

#include <stdbool.h>

#include "../types/nsk_wav_ctx_marks.h"

/*!
 * \brief  Selects dense marks with relaxed period-transition thresholds.
 *
 * \param[in] mark  Quantized mark
 * \return True if the mark passes dense period rescue thresholds
 */
bool nsk_wav_sl_bydenseperiod(
    const struct nsk_wav_mark *mark
);
