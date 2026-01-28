#include <getopt.h>

#include "../../arguments/processors/nsk_option_input_palette_back.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets input background palette file
 */
void nsk_option_input_palette_back(void) {
    nsk_options_program.input.palettes.back = optarg;
}
