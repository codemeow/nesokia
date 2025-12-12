#include <getopt.h>

#include "../../arguments/processors/nsk_option_paldirectory.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets the output palettes directory
 */
void nsk_option_paldirectory(void) {
    nsk_options_program.directory.palettes = optarg;
}