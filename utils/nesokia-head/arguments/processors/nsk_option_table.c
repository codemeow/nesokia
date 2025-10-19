#include "../../arguments/processors/nsk_option_table.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Produce table output instead of detailed description
 */
void nsk_option_table(void) {
    nsk_options_program.table = true;
}