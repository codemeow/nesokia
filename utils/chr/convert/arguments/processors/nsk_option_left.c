#include <getopt.h>

#include "../../arguments/processors/nsk_option_left.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets the input name for the left nametable
 */
void nsk_option_left(void) {
    nsk_options_program.input.left = optarg;
}