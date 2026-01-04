#include <getopt.h>

#include "../../arguments/processors/nsk_option_colors.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets the PPU colors input .pal file
 */
void nsk_option_colors(void) {
    nsk_options_program.input.colors = optarg;
}