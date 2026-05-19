#ifndef NSK_UTIL_MAX
#define NSK_UTIL_MAX

/*!
 * \brief  Simple MAX macro
 *
 * \warning Calculates both arguments twice
 *
 * \param  x  First argument
 * \param  y  Second argument
 * \return The biggest of two arguments
 */
#define NSK_MAX(x, y) ((x) > (y) ? (x) : (y))

#endif