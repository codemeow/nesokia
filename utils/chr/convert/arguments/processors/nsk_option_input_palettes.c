#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_input_palettes.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets input palettes file
 */
void nsk_option_input_palettes(void) {
    if (nsk_options_program.input.palettes.both) {
        nsk_err(
            "There must be only one `-p`/`--input-palettes` option provided\n"
        );
        exit(EXIT_FAILURE);
    }

    nsk_options_program.input.palettes.both = optarg;
}
