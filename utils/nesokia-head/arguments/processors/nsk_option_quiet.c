#include "../../arguments/processors/nsk_option_quiet.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets the "quiet" mode, producing no output but errors
 */
void nsk_option_quiet(void) {
    nsk_options_program.quiet = true;
}