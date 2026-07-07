#pragma once

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
);