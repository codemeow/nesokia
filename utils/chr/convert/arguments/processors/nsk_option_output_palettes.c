#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_output_palettes.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets output palettes file
 */
enum nsk_args_result nsk_option_output_palettes(void) {
    if (nsk_options_program.output.palettes.both) {
        nsk_err(
            "There must be only one `-P`/`--output-palettes` option provided\n"
        );
        return NSK_ARGS_EXIT_FAILURE;
    }

    nsk_options_program.output.palettes.both = optarg;

    return NSK_ARGS_CONTINUE;
}
