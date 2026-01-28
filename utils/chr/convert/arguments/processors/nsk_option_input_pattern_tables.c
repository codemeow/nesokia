#include <getopt.h>

#include "../../arguments/processors/nsk_option_input_pattern_tables.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets input pattern tables file
 */
void nsk_option_input_pattern_tables(void) {
    nsk_options_program.input.pattables.both = optarg;
}
