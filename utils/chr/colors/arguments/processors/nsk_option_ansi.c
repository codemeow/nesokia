#include "../../arguments/processors/nsk_option_ansi.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets the ANSI output flag
 */
void nsk_option_ansi(void) {
    nsk_options_program.output.ansi = true;
}