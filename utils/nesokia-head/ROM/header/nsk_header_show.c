#include <stddef.h>

#include "../../ROM/header/nsk_header_show.h"
#include "../../ROM/header/nsk_header_table.h"
#include "../../output/nsk_output_inf.h"

/*!
 * \brief  Fancy prints the header data
 *
 * \param[in] filename The ROM filename
 * \param[in]  data  The header data
 */
void nsk_header_show(
    const char *filename,
    const struct nsk_header_data *data
) {
    enum nsk_table_category category = NSK_CATEGORY_NONE;

    nsk_inf("# %s\n", filename);

    for (size_t i = 0; i < nsk_header_tablesize; i++) {
        if (nsk_header_table[i].category != category) {
            category = nsk_header_table[i].category;
            nsk_inf(" - %s:\n", nsk_category_name(category));
        }

        nsk_inf(
            "     - %-20s: %s\n",
            nsk_header_table[i].description,
            nsk_header_table[i].conv_long(
                (char *)data + nsk_header_table[i].shift
            )
        );
    }

    nsk_inf("\n");
}