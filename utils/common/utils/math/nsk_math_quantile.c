#include <stdlib.h>
#include <string.h>
#include <alloca.h>
#include <math.h>

#include "nsk_math_quantile.h"
#include "../log/nsk_log_err.h"

/*!
 * \brief  Compares two doubles for qsort
 *
 * \param[in] a   Left value
 * \param[in] b   Right value
 * \return  Comparison result
 */
static int _compare(const void *a, const void *b) {
    double x = *(const double *)a;
    double y = *(const double *)b;

    return (x > y) - (x < y);
}

/*!
 * \brief  Returns an interpolated quantile value
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
) {
    static const size_t maxstacklimit = 32;

    if (p < 0.0 || p > 1.0) {
        return false;
    }

    const size_t memsize = count * sizeof(*values);

    if (!values || count == 0 || !result) {
        nsk_err(
            "Cannot calculate percentile value for "
            "empty array or to nonexistent variable"
        );
        return false;
    }

    double *sorted = NULL;

    if (count <= maxstacklimit) {
        sorted = alloca(memsize);

    } else {
        sorted = malloc(memsize);
    }

    if (!sorted) {
        nsk_err(
            "Cannot allocate memory for percentile calculation"
        );
        return false;
    }

    memcpy(sorted, values, memsize);
    qsort(sorted, count, sizeof(*values), _compare);

    const double pos = p * (count - 1);
    const size_t lo = floor(pos);
    const size_t hi = ceil(pos);
    const double frac = pos - lo;
    *result = sorted[lo] * (1.0 - frac) + sorted[hi] * frac;

    if (count > maxstacklimit) {
        free(sorted);
    }

    return true;
}
