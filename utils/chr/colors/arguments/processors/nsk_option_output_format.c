#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_output_format.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Overrides the deduced output format
 */
void nsk_option_output_format(void) {
    if (nsk_options_program.output.format != NSK_OPTION_OUTPUT_AUTO) {
        nsk_err(
            "There must be only one `-O`/`--output-format` option provided"
        );
        exit(EXIT_FAILURE);
    }

    enum nsk_option_outputs format = nsk_output_format(optarg);
    if (format == NSK_OPTION_OUTPUT_AUTO) {
        nsk_err(
            "Unknown output format option: %s\n"
            "See '--help' for the list of options\n",
            optarg
        );
        exit(EXIT_FAILURE);
    }

    nsk_options_program.output.format = format;
}