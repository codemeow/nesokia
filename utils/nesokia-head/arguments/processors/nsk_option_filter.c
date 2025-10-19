#include <getopt.h>
#include "../../arguments/processors/nsk_option_filter.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets the filter string for the scan process
 */
void nsk_option_filter(void) {
    nsk_options_program.filter = optarg;
}