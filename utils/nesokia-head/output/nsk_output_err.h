#ifndef NSK_OUTPUT_ERR
#define NSK_OUTPUT_ERR

#include "../platform/nsk_platform_format.h"

/*!
 * \brief  Outputs message to stderr
 *
 * \param[in] fmt        The message formatter
 * \param[in] <unnamed>  Formatter arguments
 */
__attribute__((format(NSK_PLATFORM_FORMAT, 1, 2)))
void nsk_err(const char *fmt, ...);

#endif