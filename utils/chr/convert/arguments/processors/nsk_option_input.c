#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_input.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets input combined template file
 */
void nsk_option_input(void) {
    if (nsk_options_program.input.full) {
        nsk_err(
            "There must be only one `-i`/`--input` option provided\n"
        );
        exit(EXIT_FAILURE);
    }

    nsk_options_program.input.full = optarg;
}
