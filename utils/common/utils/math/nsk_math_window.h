#pragma once

#include <stddef.h>

/*!
 * \brief  Returns the Hann window coefficient for an index
 *
 * The Hann window is defined as:
 *
 *     0.5 - 0.5 * cos(2*pi*index/(count - 1))
 *
 * \param[in] index  Position inside the window
 * \param[in] count  Number of window samples
 * \return    Hann window coefficient
 */
double nsk_math_window_hannd(
    size_t index,
    size_t count
);
