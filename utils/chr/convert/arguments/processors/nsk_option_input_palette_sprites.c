#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_input_palette_sprites.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets input sprites palette file
 */
enum nsk_args_result nsk_option_input_palette_sprites(void) {
    if (nsk_options_program.input.palettes.sprites) {
        nsk_err(
            "There must be only one `-s`/`--input-palette-sprites` option provided\n"
        );
        return NSK_ARGS_EXIT_FAILURE;
    }

    nsk_options_program.input.palettes.sprites = optarg;

    return NSK_ARGS_CONTINUE;
}
