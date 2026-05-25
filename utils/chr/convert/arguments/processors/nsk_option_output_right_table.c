#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_output_right_table.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets output right pattern table file
 */
enum nsk_args_result nsk_option_output_right_table(void) {
    if (nsk_options_program.output.pattables.right) {
        nsk_err(
            "There must be only one `-R`/`--output-right-table` option provided\n"
        );
        return NSK_ARGS_EXIT_FAILURE;
    }

    nsk_options_program.output.pattables.right = optarg;

    return NSK_ARGS_CONTINUE;
}
