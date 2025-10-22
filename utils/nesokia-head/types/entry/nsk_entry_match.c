#include <stdlib.h>
#include <string.h>

#include "../../types/entry/nsk_entry_match.h"

#include "../../arguments/nsk_args_options.h"
#include "../../types/header/nsk_header_table.h"
#include "../../types/pair/nsk_pair_type.h"
#include "../../utils/log/nsk_log_err.h"

/*!
 * \brief  Matches against a single filter
 *
 * \param[in]  header The header
 * \param[in]  pair   The pair with the filter options
 * \return True if the  */
static bool _pair_match(
    const struct nsk_header_data *header,
    const struct nsk_pair        *pair
) {
    for (size_t i = 0; i < nsk_header_tablesize; i++) {
        if (strcmp(pair->name, nsk_header_table[i].shortcut) == 0) {
            return nsk_header_table[i].match(
                (char *)header + nsk_header_table[i].shift,
                pair->value,
                pair->operator
            );
        }
    }

    nsk_err("Error: unknown field name in filter: %s\n", pair->name);
    abort();
}

/*!
 * \brief  Checks if the provided header matches the set filters
 *
 * \param[in]  header  The header
 * \return True if the header matches the provided filters
 */
bool nsk_entry_match(const struct nsk_header_data *header) {
    if (!nsk_options_program.filter) {
        /* No filters set */
        return true;
    }

    struct nsk_pair *pair = nsk_options_program.filter;
    while (pair) {
        if (!_pair_match(header, pair)) {
            return false;
        }

        pair = nsk_pair_next(pair);
    }

    return true;
}