#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "nsk_wav_dc_marks.h"

/*!
 * \brief  Decoder boundary cost table.
 */
struct nsk_wav_dc_boundaries {
    size_t count;  /*!< Number of frame boundary costs */
    double *costs; /*!< Cost for each frame boundary */
};

/*!
 * \brief  Releases memory owned by boundary costs.
 *
 * \param[in,out] boundaries  Boundary cost table to clear
 */
void nsk_wav_dc_boundaries_free(
    struct nsk_wav_dc_boundaries *boundaries
);

/*!
 * \brief  Returns a boundary cost.
 *
 * \param[in] boundaries  Boundary cost table
 * \param[in] frame       Frame boundary index
 * \return Boundary cost, or 0 when frame is outside the table
 */
double nsk_wav_dc_boundaries_cost(
    const struct nsk_wav_dc_boundaries *boundaries,
    size_t frame
);

/*!
 * \brief  Builds boundary costs from decoder mark evidence.
 *
 * \param[in]  marks       Decoder marks and by-frame lookup arrays
 * \param[out] boundaries  Boundary cost table to fill
 * \return True if boundary costs were built successfully
 */
bool nsk_wav_dc_boundaries_build(
    const struct nsk_wav_dc_marks *marks,
    struct nsk_wav_dc_boundaries *boundaries
);
