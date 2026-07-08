#pragma once

#include <stdbool.h>

#include "../types/nsk_wav_ctx_marks.h"

/*!
 * \brief  Selects a mark by one strong period solver candidate.
 *
 * \param[in] mark  Quantized mark
 * \return True if the mark has a strong real period candidate
 */
bool nsk_wav_sl_bystrength(
    const struct nsk_wav_mark *mark
);
