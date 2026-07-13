#ifndef NSK_PLATFORM_POSIX
#define NSK_PLATFORM_POSIX

/*!
 * Sets the used POSIX standard to unlock required functions
 */
#define _POSIX_C_SOURCE 200112L

/*!
 * Sets the extended list of functions, using the GNU-extensions
 *
 * \note Some functions will change the behavior or signature (ex. basename)
 */
#define _GNU_SOURCE

#endif