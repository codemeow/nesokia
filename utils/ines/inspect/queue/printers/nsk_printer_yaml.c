#include <string.h>
#include <nsk_util_meta.h>

#include "../../queue/printers/nsk_printer_yaml.h"
#include "../../queue/printers/nsk_printer_match.h"
#include "../../types/entry/nsk_entry_storage.h"
#include "../../types/entry/nsk_entry_type.h"
#include "../../types/header/nsk_header_data.h"
#include "../../types/header/nsk_header_table.h"

/*!
 * The longest shortcut name
 */
static size_t shortcut_maxlen = 0;

/*!
 * \brief  Precalculations for formating
 */
void _print_prerequisites(void) {
    for (size_t i = 0; i < nsk_header_tablesize; i++) {
        size_t len = strlen(nsk_header_table[i].shortcut);
        if (len > shortcut_maxlen) {
            shortcut_maxlen = len;
        }
    }
}

/*!
 * \brief  Prints one single entry
 *
 * \param[in] filename  The filename
 * \param[in] data      The header data
 */
static void _print_entry(
    const char *filename,
    const struct nsk_header_data *data
) {
    enum nsk_table_category category = NSK_CATEGORY_NONE;

    nsk_inf("- \"filename\": \"%s\"\n", filename);

    for (size_t i = 0; i < nsk_header_tablesize; i++) {
        if (!nsk_printer_match(nsk_header_table[i].shortcut)) {
            continue;
        }

        if (nsk_header_table[i].category != category) {
            category = nsk_header_table[i].category;
            nsk_inf("  \"%s\":\n", nsk_category_json(category));
        }

        nsk_inf(
            "    \"%s\": %*s{ ",
            nsk_header_table[i].shortcut,
            (int)(shortcut_maxlen - strlen(nsk_header_table[i].shortcut)),
            ""
        );

        nsk_inf(
            "raw: %s, ",
            nsk_header_table[i].conv_json(
                (char *)data + nsk_header_table[i].shift
            )
        );
        nsk_inf(
            "display: \"%s\" }\n",
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
    struct nsk_entry *entry = nsk_entry_storage;
    while (entry) {
        struct nsk_entry *next = nsk_entry_next(entry);

        _print_entry(entry->filename, &entry->data);

        entry = next;
    }
}

/*!
 * \brief  Prints the ROMs information as an YAML document
 */
void nsk_printer_yaml(void) {
    _print_prerequisites();
    _print_entries();
}