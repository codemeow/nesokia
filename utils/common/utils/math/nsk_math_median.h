#pragma once

#include <stddef.h>
#include <stdbool.h>

/*!
 * \brief  Returns the median value
 *
 * \param[in]  values Array of values
 * \param[in]  count  Number of values in the array
 * \param[out] median Found median value
 * \return    True if the median value is successfully found
 */
bool nsk_math_mediand(
    const double *values,
    size_t        count,
    double       *median
);