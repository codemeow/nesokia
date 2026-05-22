#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_input_pattern_tables.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets input pattern tables file
 */
void nsk_option_input_pattern_tables(void) {
    if (nsk_options_program.input.pattables.both) {
        nsk_err(
            "There must be only one `-t`/`--input-pattern-tables` option provided\n"
        );
        exit(EXIT_FAILURE);
    }

    nsk_options_program.input.pattables.both = optarg;
}
