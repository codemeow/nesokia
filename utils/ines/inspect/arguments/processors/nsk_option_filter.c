#include <getopt.h>
#include <inttypes.h>

#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_filter.h"
#include "../../arguments/nsk_args_options.h"
#include "../../types/header/nsk_header_table.h"

/*!
 * \brief  Sets the filter string for the scan process
 */
void nsk_option_filter(void) {
    const char **shorts =
        nsk_util_malloc(sizeof(*shorts) * (nsk_header_tablesize + 1));

    for (size_t i = 0; i < nsk_header_tablesize; i++) {
        shorts[i] = nsk_header_table[i].shortcut;
    }
    shorts[nsk_header_tablesize] = NULL;

    nsk_options_program.filter = nsk_pair_parse(
        optarg,
        NSK_PAIR_NAME | NSK_PAIR_OPERATOR | NSK_PAIR_VALUE,
        shorts
    );

    free(shorts);

    if (!nsk_options_program.filter) {
        exit(EXIT_FAILURE);
    }
}
