#include <getopt.h>

#include "../../arguments/processors/nsk_option_output_pattern_tables.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets output pattern tables file
 */
void nsk_option_output_pattern_tables(void) {
    nsk_options_program.output.pattables.both = optarg;
}
