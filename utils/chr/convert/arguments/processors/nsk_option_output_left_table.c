#include <getopt.h>

#include "../../arguments/processors/nsk_option_output_left_table.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets output left pattern table file
 */
void nsk_option_output_left_table(void) {
    nsk_options_program.output.pattables.left = optarg;
}
