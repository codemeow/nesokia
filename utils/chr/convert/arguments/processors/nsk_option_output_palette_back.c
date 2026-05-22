#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_output_palette_back.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets output background palette file
 */
void nsk_option_output_palette_back(void) {
    if (nsk_options_program.output.palettes.back) {
        nsk_err(
            "There must be only one `-B`/`--output-palette-back` option provided\n"
        );
        exit(EXIT_FAILURE);
    }

    nsk_options_program.output.palettes.back = optarg;
}
