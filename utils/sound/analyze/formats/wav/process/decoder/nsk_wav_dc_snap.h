#pragma once

#include <stdbool.h>

#include "nsk_wav_dc_framecosts.h"
#include "nsk_wav_dc_sparse.h"

/*!
 * \brief  Applies default-enabled post-DP boundary adjustments.
 *
 * \param[in]      framecosts  Per-frame decoder evidence
 * \param[in,out]  intervals   Raw sparse intervals to adjust
 * \return True if snap passes completed successfully
 */
bool nsk_wav_dc_snap_apply(
    const struct nsk_wav_dc_framecosts *framecosts,
    struct nsk_wav_dc_intervals *intervals
);
