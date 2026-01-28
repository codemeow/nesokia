#include <getopt.h>

#include "../../arguments/processors/nsk_option_input_palettes.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets input palettes file
 */
void nsk_option_input_palettes(void) {
    nsk_options_program.input.palettes.both = optarg;
}
