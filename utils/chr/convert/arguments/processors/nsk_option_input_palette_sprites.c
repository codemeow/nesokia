#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_input_palette_sprites.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets input sprites palette file
 */
void nsk_option_input_palette_sprites(void) {
    if (nsk_options_program.input.palettes.sprites) {
        nsk_err(
            "There must be only one `-s`/`--input-palette-sprites` option provided\n"
        );
        exit(EXIT_FAILURE);
    }

    nsk_options_program.input.palettes.sprites = optarg;
}
