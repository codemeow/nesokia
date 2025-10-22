#include <getopt.h>

#include "../../arguments/processors/nsk_option_table.h"
#include "../../arguments/nsk_args_options.h"
#include "../../list/pair/nsk_pair_parse.h"

/*!
 * \brief  Produce table output instead of detailed description
 */
void nsk_option_table(void) {
    nsk_options_program.table = true;
    if (optarg) {
        nsk_options_program.fields = nsk_pair_parse(
            optarg,
            NSK_PAIR_NAME
        );
    }
}