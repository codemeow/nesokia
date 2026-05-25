#ifndef NSK_UTIL_MIN
#define NSK_UTIL_MIN

/*!
 * \brief  Simple MIN macro
 *
 * \warning Calculates both arguments twice
 *
 * \param  x  First argument
 * \param  y  Second argument
 * \return The smallest of two arguments
 */
#define NSK_MIN(x, y) ((x) < (y) ? (x) : (y))

#endif