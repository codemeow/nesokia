#include <getopt.h>

#include "../../arguments/processors/nsk_option_template.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets the input name for the template image
 */
void nsk_option_template(void) {
    nsk_options_program.input.template = optarg;
}