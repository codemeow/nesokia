#include <getopt.h>

#include "../../arguments/processors/nsk_option_terminalansi.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Enables the colored output
 */
void nsk_option_terminalansi(void) {
    nsk_options_program.output.colored = true;
}