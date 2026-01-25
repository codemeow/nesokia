#include <getopt.h>

#include "../../arguments/processors/nsk_option_output.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets output filename
 */
void nsk_option_output(void) {
    nsk_options_program.output.file = optarg;
}