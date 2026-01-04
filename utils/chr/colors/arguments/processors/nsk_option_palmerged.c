#include <getopt.h>

#include "../../arguments/processors/nsk_option_palmerged.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets the merged palette file
 */
void nsk_option_palmerged(void) {
    nsk_options_program.input.palmerged = optarg;
}