#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_output.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets output filename
 */
void nsk_option_output(void) {
    if (nsk_options_program.output.file) {
        nsk_err(
            "There must be only one `-o`/`--output` option provided\n"
        );
        exit(EXIT_FAILURE);
    }

    nsk_options_program.output.file = optarg;
}
