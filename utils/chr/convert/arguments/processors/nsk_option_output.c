#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_output.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets output combined template file
 */
void nsk_option_output(void) {
    if (nsk_options_program.output.full) {
        nsk_err(
            "There must be only one `-O`/`--output` option provided\n"
        );
        exit(EXIT_FAILURE);
    }

    nsk_options_program.output.full = optarg;
}
