#include <getopt.h>

#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_keys.h"
#include "../../arguments/nsk_args_options.h"
#include "../../types/header/nsk_header_table.h"

/*!
 * \brief Select a set of keys to show
 */
enum nsk_args_result nsk_option_keys(void) {
    const char **shorts =
        nsk_util_malloc(sizeof(*shorts) * (nsk_header_tablesize + 1));

    for (size_t i = 0; i < nsk_header_tablesize; i++) {
        shorts[i] = nsk_header_table[i].shortcut;
    }
    shorts[nsk_header_tablesize] = NULL;

    if (optarg) {
        nsk_options_program.keys = nsk_pair_parse(
            optarg,
            NSK_PAIR_NAME,
            shorts
        );
    }

    free(shorts);

    if (optarg && !nsk_options_program.keys) {
        return NSK_ARGS_EXIT_FAILURE;
    }

    return NSK_ARGS_CONTINUE;
}
