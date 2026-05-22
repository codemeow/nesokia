#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_input_ppu_colors.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets input PPU colors file
 */
enum nsk_args_result nsk_option_input_ppu_colors(void) {
    if (nsk_options_program.input.ppucolors) {
        nsk_err(
            "There must be only one `-c`/`--input-ppu-colors` option provided\n"
        );
        return NSK_ARGS_EXIT_FAILURE;
    }

    nsk_options_program.input.ppucolors = optarg;

    return NSK_ARGS_CONTINUE;
}
