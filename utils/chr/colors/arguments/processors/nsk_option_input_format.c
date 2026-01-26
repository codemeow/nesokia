#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_input_format.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Overrides the deduced input format
 */
void nsk_option_input_format(void) {
    if (nsk_options_program.input.format != NSK_OPTION_INPUT_AUTO) {
        nsk_err(
            "There must be only one `-I`/`--input-format` option provided"
        );
        exit(EXIT_FAILURE);
    }

    enum nsk_option_inputs format = nsk_input_format(optarg);
    if (format == NSK_OPTION_INPUT_AUTO) {
        nsk_err(
            "Unknown input format option: %s\n"
            "See '--help' for the list of options\n",
            optarg
        );
        exit(EXIT_FAILURE);
    }

    nsk_options_program.input.format = format;
}