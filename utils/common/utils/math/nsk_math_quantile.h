#pragma once

#include <stdbool.h>
#include <stddef.h>

/*!
 * \brief  Returns an interpolated quantile value
 *
 * The input percentile is expressed as a normalized quantile in the inclusive
 * range [0.0, 1.0].  For example, 0.10 selects the 10th percentile and 0.50
 * selects the median.  The input array is copied before sorting, so the
 * original order is preserved.
 *
 * \param[in]  p       Quantile in the [0.0, 1.0] range
 * \param[in]  values  Array of values
 * \param[in]  count   Number of values in the array
 * \param[out] result  Interpolated quantile value
 * \return     True if the quantile was calculated
 */
bool nsk_math_quantiled(
    double        p,
    const double *values,
    size_t        count,
    double       *result
);
