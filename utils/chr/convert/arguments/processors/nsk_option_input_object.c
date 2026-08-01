#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_input_object.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets input background object PNG file
 */
enum nsk_args_result nsk_option_input_object(void) {
    if (nsk_options_program.input.object) {
        nsk_err(
            "There must be only one `-o`/`--input-object` option provided\n"
        );
        return NSK_ARGS_EXIT_FAILURE;
    }

    nsk_options_program.input.object = optarg;

    return NSK_ARGS_CONTINUE;
}
