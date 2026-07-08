#pragma once

#include <stdbool.h>

#include "../types/nsk_wav_ctx_marks.h"

/*!
 * \brief  Selects a mark by grid-close energy evidence.
 *
 * \param[in] mark  Quantized mark
 * \return True if the mark has enough energy evidence
 */
bool nsk_wav_sl_byenergy(
    const struct nsk_wav_mark *mark
);
