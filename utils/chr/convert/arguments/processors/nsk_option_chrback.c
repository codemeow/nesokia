#include <getopt.h>

#include "../../arguments/processors/nsk_option_chrback.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets the output name for the back CHR data
 */
void nsk_option_chrback(void) {
    nsk_options_program.output.nametable[NSK_PLANE_BACKGROUND] = optarg;
}