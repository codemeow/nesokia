#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_output_object_attributes.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets output background attribute file
 */
enum nsk_args_result nsk_option_output_object_attributes(void) {
    if (nsk_options_program.output.object_attributes) {
        nsk_err(
            "There must be only one `-G`/`--output-object-attributes` option provided\n"
        );
        return NSK_ARGS_EXIT_FAILURE;
    }

    nsk_options_program.output.object_attributes = optarg;

    return NSK_ARGS_CONTINUE;
}
