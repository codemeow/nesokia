#include <stdbool.h>
#include <stdio.h>

#include "nsk_util_multimap.h"

/*!
 * \brief  Lookups the values from the multimap
 *
 * Returns the merged string with the \p separator as the values separator
 *
 * \param[in] multimap  The multimap
 * \param[in] key       The key
 * \param[in] separator The values separator
 * \return Static string with merged values or the empty string
 */
const char *nsk_multimap_lookup(
    const struct nsk_multimap *multimap,
    int key,
    const char *separator
) {
    static char value[1024];
    bool first  = true;
    bool found  = false;
    int pos     = 0;
    int count;

    const struct nsk_multimap *entry = multimap;
    while (entry->value) {
        if (entry->key == key) {

            found = true;

            if (!first) {
                count = snprintf(
                    value + pos,
                    sizeof(value) - pos,
                    "%s",
                    separator
                );
                if (count > 0 && count < (int)sizeof(value) - pos) {
                    pos += count;
                } else {
                    return "";
                }
            }

            count = snprintf(value + pos, sizeof(value) - pos, "%s", entry->value);
            if (count > 0 && count < (int)sizeof(value) - pos) {
                pos += count;
            } else {
                return "";
            }

            first = false;
        }

        entry++;
    }

    if (!found) {
        snprintf(
            value,
            sizeof(value),
            "Unknown (%d)",
            key
        );
    }

    return value;
}