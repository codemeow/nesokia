#include <getopt.h>

#include "../../arguments/processors/nsk_option_palback.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets the background palette file
 */
void nsk_option_palback(void) {
    nsk_options_program.input.palback = optarg;
}