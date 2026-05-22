#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_output_ppu_colors.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets output PPU colors file
 */
enum nsk_args_result nsk_option_output_ppu_colors(void) {
    if (nsk_options_program.output.ppucolors) {
        nsk_err(
            "There must be only one `-C`/`--output-ppu-colors` option provided\n"
        );
        return NSK_ARGS_EXIT_FAILURE;
    }

    nsk_options_program.output.ppucolors = optarg;

    return NSK_ARGS_CONTINUE;
}
