#include <stdarg.h>
#include <stdio.h>

#include "../log/nsk_log_inf.h"

/*!
 * \brief Silences the non-error output
 */
bool nsk_log_quiet = false;

/*!
 * \brief  Outputs message to stdout
 *
 * \param[in] fmt        The message formatter
 * \param[in] <unnamed>  Formatter arguments
 */
void nsk_inf(const char *fmt, ...) {
    if (nsk_log_quiet) {
        return;
    }

    va_list args;
    va_start(args, fmt);

    vfprintf(
        stdout,
        fmt,
        args
    );

    va_end(args);
}