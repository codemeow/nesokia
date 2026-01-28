#include <getopt.h>

#include "../../arguments/processors/nsk_option_output_ppu_colors.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets output PPU colors file
 */
void nsk_option_output_ppu_colors(void) {
    nsk_options_program.output.ppucolors = optarg;
}
