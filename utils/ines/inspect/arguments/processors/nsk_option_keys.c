#include <getopt.h>

#include "../../arguments/processors/nsk_option_keys.h"

#include "../../arguments/nsk_args_options.h"
#include "../../types/pair/nsk_pair_parse.h"

/*!
 * \brief Select a set of keys to show
 */
void nsk_option_keys(void) {
    if (optarg) {
        nsk_options_program.keys = nsk_pair_parse(
            optarg,
            NSK_PAIR_NAME
        );
    }
}