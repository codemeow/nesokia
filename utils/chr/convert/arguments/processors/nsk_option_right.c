#include <getopt.h>

#include "../../arguments/processors/nsk_option_right.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets the input name for the right nametable
 */
void nsk_option_right(void) {
    nsk_options_program.input.right = optarg;
}