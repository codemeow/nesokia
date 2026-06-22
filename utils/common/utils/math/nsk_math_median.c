#include <string.h>
#include <stdlib.h>
#include <alloca.h>

#include "../log/nsk_log_err.h"

#include "nsk_math_median.h"

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
) {
    static const size_t maxstacklimit = 32;
    const size_t memsize = count * sizeof(*values);

    if (!values || count == 0 || !median) {
        nsk_err(
            "Cannot calculate median value for "
            "empty array or to nonexistent variable"
        );
        return false;
    }

    if (count == 1) {
        *median = values[0];
        return true;
    }

    if (count == 2) {
        *median = values[0] / 2.0 + values[1] / 2.0;
        return true;
    }

    double *sorted = NULL;

    if (count <= maxstacklimit) {
        sorted = alloca(memsize);

    } else {
        sorted = malloc(memsize);
    }

    if (!sorted) {
        nsk_err(
            "Cannot allocate memory for median calculation"
        );
        return false;
    }

    memcpy(sorted, values, memsize);
    qsort(sorted, count, sizeof(*values), _compare);

    if (count % 2 == 1) {
        *median = sorted[count / 2];

    } else {
        *median =
            sorted[count / 2 - 1] / 2.0 +
            sorted[count / 2    ] / 2.0;
    }

    if (count > maxstacklimit) {
        free(sorted);
    }

    return true;
}
