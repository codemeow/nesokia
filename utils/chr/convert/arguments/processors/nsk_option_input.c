#include <getopt.h>

#include "../../arguments/processors/nsk_option_input.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets input combined template file
 */
void nsk_option_input(void) {
    nsk_options_program.input.full = optarg;
}
