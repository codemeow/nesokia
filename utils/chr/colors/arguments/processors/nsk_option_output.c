#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_output.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets output filename
 */
enum nsk_args_result nsk_option_output(void) {
    if (nsk_options_program.output.file) {
        nsk_err(
            "There must be only one `-o`/`--output` option provided\n"
        );
        return NSK_ARGS_EXIT_FAILURE;
    }

    nsk_options_program.output.file = optarg;

    return NSK_ARGS_CONTINUE;
}
