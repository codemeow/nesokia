#include <getopt.h>

#include "../../arguments/processors/nsk_option_input_ppu_colors.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets input PPU colors file
 */
void nsk_option_input_ppu_colors(void) {
    nsk_options_program.input.ppucolors = optarg;
}
