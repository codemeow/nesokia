#include <getopt.h>

#include "../../arguments/processors/nsk_option_palpng.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets the palettes output PNG filename
 */
void nsk_option_palpng(void) {
    nsk_options_program.output.palettes = optarg;
}