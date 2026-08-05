#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_output_object.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets output background CHR tiles file
 */
enum nsk_args_result nsk_option_output_object(void) {
    if (nsk_options_program.output.object) {
        nsk_err(
            "There must be only one `-D`/`--output-object` option provided\n"
        );
        return NSK_ARGS_EXIT_FAILURE;
    }

    nsk_options_program.output.object = optarg;

    return NSK_ARGS_CONTINUE;
}
