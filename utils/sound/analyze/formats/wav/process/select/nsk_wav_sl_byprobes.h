#pragma once

#include <stdbool.h>

#include "../types/nsk_wav_ctx_marks.h"

/*!
 * \brief  Selects dense marks supported only by transition probes.
 *
 * \param[in] mark  Quantized mark
 * \return True if the mark passes dense probe rescue thresholds
 */
bool nsk_wav_sl_byprobes(
    const struct nsk_wav_mark *mark
);
