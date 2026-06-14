#include <getopt.h>

#include <nsk_util_meta.h>

#include "nsk_option_input.h"
#include "../nsk_args_options.h"

/*!
 * \brief  Set input filename
 */
enum nsk_args_result nsk_option_input(void) {
    if (nsk_options_program.input.file) {
        nsk_err(
            "There must be only one `-i`/`--input` option provided\n"
        );
        return NSK_ARGS_EXIT_FAILURE;
    }

    nsk_options_program.input.file = optarg;

    return NSK_ARGS_CONTINUE;
}