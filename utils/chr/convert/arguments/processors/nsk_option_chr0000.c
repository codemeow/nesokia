#include <getopt.h>

#include "../../arguments/processors/nsk_option_chr0000.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets the output name for the left CHR data ($0000)
 */
void nsk_option_chr0000(void) {
    nsk_options_program.output.chr0000 = optarg;
}