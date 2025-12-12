#include <getopt.h>

#include "../../arguments/processors/nsk_option_outdirectory.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets the output directory
 */
void nsk_option_outdirectory(void) {
    nsk_options_program.directory.output = optarg;
}