#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "nsk_wav_dc_boundaries.h"
#include "nsk_wav_dc_framecosts.h"

/*!
 * \brief  Raw sparse decoder interval.
 */
struct nsk_wav_dc_interval {
    size_t framestart; /*!< Inclusive interval start frame */
    size_t frameend;   /*!< Exclusive interval end frame */
    size_t corestart;  /*!< Inclusive weighted core start frame */
    size_t coreend;    /*!< Exclusive weighted core end frame */

    int midi; /*!< MIDI note, or -1 for rest */

    double emission; /*!< Interval state cost */
    double boundary; /*!< Start boundary cost */
    double total;    /*!< Total DP cost at interval end */
};

/*!
 * \brief  Raw sparse decoder interval list.
 */
struct nsk_wav_dc_intervals {
    size_t count; /*!< Number of raw intervals */
    struct nsk_wav_dc_interval *list; /*!< Raw interval list */
};

/*!
 * \brief  Releases memory owned by a raw interval list.
 *
 * \param[in,out] intervals  Raw interval list to clear
 */
void nsk_wav_dc_intervals_free(
    struct nsk_wav_dc_intervals *intervals
);

/*!
 * \brief  Chooses the best decoder state for one frame interval.
 *
 * \param[in]  framecosts  Per-frame state costs
 * \param[in]  framestart  Inclusive frame start
 * \param[in]  frameend    Exclusive frame end
 * \param[out] interval    Interval result to fill
 * \return True if the interval was scored successfully
 */
bool nsk_wav_dc_sparse_state(
    const struct nsk_wav_dc_framecosts *framecosts,
    size_t framestart,
    size_t frameend,
    struct nsk_wav_dc_interval *interval
);

/*!
 * \brief  Decodes raw sparse intervals with global dynamic programming.
 *
 * \param[in]  framecosts  Per-frame state costs
 * \param[in]  boundaries  Boundary costs
 * \param[out] intervals   Raw decoded interval list
 * \return True if sparse decoding completed successfully
 */
bool nsk_wav_dc_sparse_decode(
    const struct nsk_wav_dc_framecosts *framecosts,
    const struct nsk_wav_dc_boundaries *boundaries,
    struct nsk_wav_dc_intervals *intervals
);
