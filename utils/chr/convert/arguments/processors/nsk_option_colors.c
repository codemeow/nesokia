#include <getopt.h>

#include "../../arguments/processors/nsk_option_colors.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets the input name for the global colors file
 */
void nsk_option_colors(void) {
    nsk_options_program.input.colors = optarg;
}