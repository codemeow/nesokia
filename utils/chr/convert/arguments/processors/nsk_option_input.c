#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_input.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets input combined template file
 */
enum nsk_args_result nsk_option_input(void) {
    if (nsk_options_program.input.full) {
        nsk_err(
            "There must be only one `-i`/`--input` option provided\n"
        );
        return NSK_ARGS_EXIT_FAILURE;
    }

    nsk_options_program.input.full = optarg;

    return NSK_ARGS_CONTINUE;
}
