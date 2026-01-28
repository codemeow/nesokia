#include <getopt.h>

#include "../../arguments/processors/nsk_option_output_palette_back.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets output background palette file
 */
void nsk_option_output_palette_back(void) {
    nsk_options_program.output.palettes.back = optarg;
}
