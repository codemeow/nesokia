#include <getopt.h>

#include "../../arguments/processors/nsk_option_input_right_table.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets input right pattern table file
 */
void nsk_option_input_right_table(void) {
    nsk_options_program.input.pattables.right = optarg;
}
