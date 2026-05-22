#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_output_left_table.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets output left pattern table file
 */
void nsk_option_output_left_table(void) {
    if (nsk_options_program.output.pattables.left) {
        nsk_err(
            "There must be only one `-L`/`--output-left-table` option provided\n"
        );
        exit(EXIT_FAILURE);
    }

    nsk_options_program.output.pattables.left = optarg;
}
