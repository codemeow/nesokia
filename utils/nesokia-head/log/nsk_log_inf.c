#include <stdio.h>
#include <stdarg.h>

#include "../arguments/nsk_args_options.h"
#include "../log/nsk_log_inf.h"

/*!
 * \brief  Outputs message to stdout
 *
 * \param[in] fmt        The message formatter
 * \param[in] <unnamed>  Formatter arguments
 */
void nsk_inf(const char *fmt, ...) {
    if (nsk_options_program.quiet) {
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