#include <getopt.h>

#include "../../arguments/processors/nsk_option_output_palettes.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets output palettes file
 */
void nsk_option_output_palettes(void) {
    nsk_options_program.output.palettes.both = optarg;
}
