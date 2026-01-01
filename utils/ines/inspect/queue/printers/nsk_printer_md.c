#include <nsk_util_meta.h>

#include "../../queue/printers/nsk_printer_md.h"

#include "../../types/entry/nsk_entry_type.h"
#include "../../types/entry/nsk_entry_storage.h"
#include "../../types/header/nsk_header_data.h"
#include "../../types/header/nsk_header_table.h"
#include "../../queue/printers/nsk_printer_match.h"

/*!
 * \brief  Prints one single entry
 *
 * \param[in] filename  The filename
 * \param[in] data      The header data
 */
static void _print_entry(
    const char *filename,
    const struct nsk_header_data *data,
    bool first
) {
    enum nsk_table_category category = NSK_CATEGORY_NONE;

    if (!first) {
        nsk_inf("---\n\n");
    }

    nsk_inf("## `%s`\n", filename);

    for (size_t i = 0; i < nsk_header_tablesize; i++) {
        if (!nsk_printer_match(nsk_header_table[i].shortcut)) {
            continue;
        }

        if (nsk_header_table[i].category != category) {
            category = nsk_header_table[i].category;
            nsk_inf("\n");
            nsk_inf("### %s\n", nsk_category_name(category));
            nsk_inf("\n");

            if (category == NSK_CATEGORY_MEMORY) {
                nsk_inf("| Type | Size |\n");
                nsk_inf("| ---- | ---- |\n");
            }
        }

        const char *fmt;
        if (category == NSK_CATEGORY_MEMORY) {
            fmt = "| %s | %s |\n";

        } else {
            fmt = "* **%s:** %s\n";
        }

        nsk_inf(
            fmt,
            nsk_header_table[i].description,
            nsk_header_table[i].conv_long(
                (char *)data + nsk_header_table[i].shift
            )
        );
    }

    nsk_inf("\n");
}

/*!
 * \brief  Prints entries.
 */
static void _print_entries(void) {
    bool first = true;

    struct nsk_entry *entry = nsk_entry_storage;
    while (entry) {
        struct nsk_entry *next = nsk_entry_next(entry);

        _print_entry(entry->filename, &entry->data, first);
        first = false;

        entry = next;
    }
}

/*!
 * \brief  Prints the ROM information as a Markdown document
 */
void nsk_printer_md(void) {
    _print_entries();
}