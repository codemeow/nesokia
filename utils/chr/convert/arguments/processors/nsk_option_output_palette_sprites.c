#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_output_palette_sprites.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets output sprites palette file
 */
void nsk_option_output_palette_sprites(void) {
    if (nsk_options_program.output.palettes.sprites) {
        nsk_err(
            "There must be only one `-S`/`--output-palette-sprites` option provided\n"
        );
        exit(EXIT_FAILURE);
    }

    nsk_options_program.output.palettes.sprites = optarg;
}
