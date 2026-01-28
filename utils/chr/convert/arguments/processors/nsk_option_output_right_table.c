#include <getopt.h>

#include "../../arguments/processors/nsk_option_output_right_table.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets output right pattern table file
 */
void nsk_option_output_right_table(void) {
    nsk_options_program.output.pattables.right = optarg;
}
