#include <getopt.h>

#include "../../arguments/processors/nsk_option_palettes.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets the input name for the palettes image
 */
void nsk_option_palettes(void) {
    nsk_options_program.input.palettes = optarg;
}