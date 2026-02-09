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
    const char **shorts = malloc(sizeof(*shorts) * nsk_header_tablesize);
    for (size_t i = 0; i < nsk_header_tablesize; i++) {
        shorts[i] = nsk_header_table[i].shortcut;
    }

    nsk_options_program.filter = nsk_pair_parse(
        optarg,
        NSK_PAIR_NAME | NSK_PAIR_OPERATOR | NSK_PAIR_VALUE,
        shorts
    );

    free(shorts);
}