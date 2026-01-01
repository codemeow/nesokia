#include <getopt.h>

#include "../../arguments/processors/nsk_option_palback.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets the ouput name for the background palette
 */
void nsk_option_palback(void) {
    nsk_options_program.output.palettes[NSK_PLANE_BACKGROUND] = optarg;
}