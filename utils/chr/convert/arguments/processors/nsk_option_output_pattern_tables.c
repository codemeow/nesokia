#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_output_pattern_tables.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets output pattern tables file
 */
void nsk_option_output_pattern_tables(void) {
    if (nsk_options_program.output.pattables.both) {
        nsk_err(
            "There must be only one `-T`/`--output-pattern-tables` option provided\n"
        );
        exit(EXIT_FAILURE);
    }

    nsk_options_program.output.pattables.both = optarg;
}
