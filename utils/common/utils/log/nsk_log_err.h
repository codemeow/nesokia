#ifndef NSK_LOG_ERR
#define NSK_LOG_ERR

/*!
 * \brief  Outputs message to stderr
 *
 * \param[in] fmt        The message formatter
 * \param[in] <unnamed>  Formatter arguments
 */
__attribute__((format(NSK_PLATFORM_FORMAT, 1, 2)))
void nsk_err(const char *fmt, ...);

#endif