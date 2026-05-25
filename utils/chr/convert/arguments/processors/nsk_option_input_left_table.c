#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_input_left_table.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets input left pattern table file
 */
enum nsk_args_result nsk_option_input_left_table(void) {
    if (nsk_options_program.input.pattables.left) {
        nsk_err(
            "There must be only one `-l`/`--input-left-table` option provided\n"
        );
        return NSK_ARGS_EXIT_FAILURE;
    }

    nsk_options_program.input.pattables.left = optarg;

    return NSK_ARGS_CONTINUE;
}
