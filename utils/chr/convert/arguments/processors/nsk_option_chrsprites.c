#include <getopt.h>

#include "../../arguments/processors/nsk_option_chrsprites.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets the output name for the sprites CHR data
 */
void nsk_option_chrsprites(void) {
    nsk_options_program.output.nametable[NSK_PLANE_SPRITES] = optarg;
}