#include <getopt.h>

#include "../../arguments/processors/nsk_option_output.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets output combined template file
 */
void nsk_option_output(void) {
    nsk_options_program.output.full = optarg;
}
