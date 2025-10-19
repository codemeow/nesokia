#include <stdio.h>
#include <stdarg.h>

#include "nsk_output_err.h"

/*!
 * \brief  Outputs message to stderr
 *
 * \param[in] fmt        The message formatter
 * \param[in] <unnamed>  Formatter arguments
 */
void nsk_err(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    vfprintf(
        stderr,
        fmt,
        args
    );

    va_end(args);
}