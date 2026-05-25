#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_input_palette_back.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets input background palette file
 */
enum nsk_args_result nsk_option_input_palette_back(void) {
    if (nsk_options_program.input.palettes.back) {
        nsk_err(
            "There must be only one `-b`/`--input-palette-back` option provided\n"
        );
        return NSK_ARGS_EXIT_FAILURE;
    }

    nsk_options_program.input.palettes.back = optarg;

    return NSK_ARGS_CONTINUE;
}
