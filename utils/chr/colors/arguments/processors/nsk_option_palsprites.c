#include <getopt.h>

#include "../../arguments/processors/nsk_option_palsprites.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets the sprites palette file
 */
void nsk_option_palsprites(void) {
    nsk_options_program.input.palsprites = optarg;
}