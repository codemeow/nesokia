#include "../../queue/printers/nsk_printer_tree.h"

#include "../../types/entry/nsk_entry_storage.h"
#include "../../types/entry/nsk_entry_type.h"
#include "../../types/header/nsk_header_table.h"
#include "../../utils/log/nsk_log_inf.h"

/*!
 * \brief  Fancy prints the header data
 *
 * \param[in] filename The ROM filename
 * \param[in]  data  The header data
 */
void _header_print(
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

/*!
 * \brief  Peints the ROMs information as human-readable tree-like view
 */
void nsk_printer_tree(void) {
    struct nsk_entry *entry = nsk_entry_storage;
    while (entry) {
        _header_print(entry->filename, &entry->data);

        entry = nsk_entry_next(entry);
    }
}