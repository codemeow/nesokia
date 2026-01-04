#include <getopt.h>

#include "../../arguments/processors/nsk_option_ppupng.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets the PPU colors PNG filename for output
 */
void nsk_option_ppupng(void) {
    nsk_options_program.output.colors = optarg;
}