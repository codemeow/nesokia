#include <getopt.h>

#include "../../arguments/processors/nsk_option_input_palette_sprites.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets input sprites palette file
 */
void nsk_option_input_palette_sprites(void) {
    nsk_options_program.input.palettes.sprites = optarg;
}
