#include <getopt.h>

#include "../../arguments/processors/nsk_option_output_palette_sprites.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets output sprites palette file
 */
void nsk_option_output_palette_sprites(void) {
    nsk_options_program.output.palettes.sprites = optarg;
}
