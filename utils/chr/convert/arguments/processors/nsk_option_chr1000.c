#include <getopt.h>

#include "../../arguments/processors/nsk_option_chr1000.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets the output name for the right CHR data ($1000)
 */
void nsk_option_chr1000(void) {
    nsk_options_program.output.chr1000 = optarg;
}