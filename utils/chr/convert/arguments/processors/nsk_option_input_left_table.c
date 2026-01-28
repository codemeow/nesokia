#include <getopt.h>

#include "../../arguments/processors/nsk_option_input_left_table.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets input left pattern table file
 */
void nsk_option_input_left_table(void) {
    nsk_options_program.input.pattables.left = optarg;
}
