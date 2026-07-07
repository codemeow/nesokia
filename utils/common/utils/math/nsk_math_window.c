#include <math.h>

#include "nsk_math_window.h"

/*!
 * \brief  Returns the Hann window coefficient for an index
 *
 * \param[in] index  Position inside the window
 * \param[in] count  Number of window samples
 * \return    Hann window coefficient
 */
double nsk_math_window_hannd(
    size_t index,
    size_t count
) {
    return
        0.5 -
        0.5 * cos(
            2.0 * M_PI *
            index /
            (count - 1)
        );
}
