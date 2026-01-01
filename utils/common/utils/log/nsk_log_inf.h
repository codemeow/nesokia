#ifndef NSK_LOG_INF
#define NSK_LOG_INF

#include <stdbool.h>

/*!
 * \brief Silences the non-error output
 */
extern bool nsk_log_quiet;

/*!
 * \brief  Outputs message to stdout
 *
 * \param[in] fmt        The message formatter
 * \param[in] <unnamed>  Formatter arguments
 */
__attribute__((format(NSK_PLATFORM_FORMAT, 1, 2)))
void nsk_inf(const char *fmt, ...);

#endif