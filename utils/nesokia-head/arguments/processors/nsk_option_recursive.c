#include "../../arguments/processors/nsk_option_recursive.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Enable recursive search when looking for files
 */
void nsk_option_recursive(void) {
    nsk_options_program.recursive = true;
}