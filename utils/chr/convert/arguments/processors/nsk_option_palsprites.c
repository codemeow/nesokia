#include <getopt.h>

#include "../../arguments/processors/nsk_option_palsprites.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets the ouput name for the sprites palette
 */
void nsk_option_palsprites(void) {
    nsk_options_program.output.palettes[NSK_PLANE_SPRITES] = optarg;
}