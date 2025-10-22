#ifndef NSK_LOG_INF
#define NSK_LOG_INF

#include "../platform/nsk_platform_format.h"

/*!
 * \brief  Outputs message to stdout
 *
 * \param[in] fmt        The message formatter
 * \param[in] <unnamed>  Formatter arguments
 */
__attribute__((format(NSK_PLATFORM_FORMAT, 1, 2)))
void nsk_inf(const char *fmt, ...);

#endif