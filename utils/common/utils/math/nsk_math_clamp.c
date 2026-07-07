#include "nsk_math_clamp.h"

#include "../base/nsk_util_max.h"
#include "../base/nsk_util_min.h"

/*!
 * \brief  Clamps double value from both ends
 *
 * \param[in] value  The value
 * \param[in] min    The minimum
 * \param[in] max    The maximum
 * \return    Clamped value
 */
double nsk_math_clampd(
    double value,
    double min,
    double max
) {
    return NSK_MIN(
        NSK_MAX(
            value,
            min
        ),
        max
    );
}