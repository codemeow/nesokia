#include <nsk_util_meta.h>

#include "../../queue/printers/nsk_printer_json.h"
#include "../../queue/printers/nsk_printer_match.h"
#include "../../types/entry/nsk_entry_storage.h"
#include "../../types/entry/nsk_entry_type.h"
#include "../../types/header/nsk_header_data.h"
#include "../../types/header/nsk_header_table.h"

/*!
 * \brief  Prints JSON header
 */
static void _print_header(void) {
    nsk_inf("[\n");
}

/*!
 * \brief  Prints JSON footer.
 */
static void _print_footer(void) {
    nsk_inf("]\n");
}

/*!
 * \brief  Checks if the current category has more matching fields
 *
 * \param[in] index     Current table index
 * \param[in] category  Current category
 *
 * \return True if another matching field exists in the same category
 */
static bool _has_next_match_in_category(
    size_t index,
    enum nsk_table_category category
) {
    for (size_t i = index + 1; i < nsk_header_tablesize; i++) {
        if (nsk_header_table[i].category != category) {
            return false;
        }

        if (nsk_printer_match(nsk_header_table[i].shortcut)) {
            return true;
        }
    }

    return false;
}

/*!
 * \brief  Prints one single entry
 *
 * \param[in] filename  The filename
 * \param[in] data      The header data
 * \param[in] last      Is this the last record?
 */
static void _print_entry(
    const char *filename,
    const struct nsk_header_data *data,
    bool last
) {
    enum nsk_table_category category = NSK_CATEGORY_NONE;

    nsk_inf("\t{\n");
    nsk_inf("\t\t\"filename\" : \"%s\",\n", filename);

    for (size_t i = 0; i < nsk_header_tablesize; i++) {
        if (!nsk_printer_match(nsk_header_table[i].shortcut)) {
            continue;
        }

        if (nsk_header_table[i].category != category) {
            if (category != NSK_CATEGORY_NONE) {
                nsk_inf("\t\t},\n");
            }
            category = nsk_header_table[i].category;
            nsk_inf("\t\t\"%s\" : {\n", nsk_category_json(category));
        }

        bool locallast = !_has_next_match_in_category(i, category);

        nsk_inf(
            "\t\t\t\"%s\" : {\n",
            nsk_header_table[i].shortcut
        );

        nsk_inf(
            "\t\t\t\t\"raw\" : %s,\n",
            nsk_header_table[i].conv_json(
                (char *)data + nsk_header_table[i].shift
            )
        );
        nsk_inf(
            "\t\t\t\t\"display\" : \"%s\"\n",
            nsk_header_table[i].conv_long(
                (char *)data + nsk_header_table[i].shift
            )
        );

        nsk_inf("\t\t\t}%s\n", locallast ? "" : ",");
    }

    nsk_inf("\t\t}\n");
    nsk_inf("\t}%s\n", last ? "" : ",");
}

/*!
 * \brief  Prints entries.
 */
static void _print_entries(void) {
    struct nsk_entry *entry = nsk_entry_storage;
    while (entry) {
        struct nsk_entry *next = nsk_entry_next(entry);

        _print_entry(entry->filename, &entry->data, next == NULL);

        entry = next;
    }
}

/*!
 * \brief  Prints the ROMs information as a JSON document
 */
void nsk_printer_json(void) {
    _print_header();
    _print_entries();
    _print_footer();
}
