#pragma once

#include <stdbool.h>

#include "../types/nsk_wav_ctx_marks.h"

/*!
 * \brief  Selects a mark by multiple real period evidences.
 *
 * \param[in] mark  Quantized mark
 * \return True if the mark has enough real period evidence
 */
bool nsk_wav_sl_bytrusted(
    const struct nsk_wav_mark *mark
);
