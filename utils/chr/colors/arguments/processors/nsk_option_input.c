#include <getopt.h>

#include "../../arguments/processors/nsk_option_input.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets input filename
 */
void nsk_option_input(void) {
    nsk_options_program.input.file = optarg;
}