#include <string.h>

#include "../../queue/printers/nsk_printer_csv.h"

#include "../../utils/log/nsk_log_inf.h"
#include "../../types/entry/nsk_entry_type.h"
#include "../../types/entry/nsk_entry_storage.h"
#include "../../types/header/nsk_header_data.h"
#include "../../types/header/nsk_header_table.h"
#include "../../queue/printers/nsk_printer_match.h"

/*!
 * \brief  Prints JSON header
 */
static void _print_header(void) {
    nsk_inf("Filename");
    for (size_t i = 0; i < nsk_header_tablesize; i++) {
        if (!nsk_printer_match(nsk_header_table[i].shortcut)) {
            continue;
        }

        nsk_inf(",");

        nsk_inf("\"%s\"", nsk_header_table[i].description);
    }
    nsk_inf("\n");
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
    nsk_inf("\"%s\"", filename);
    for (size_t i = 0; i < nsk_header_tablesize; i++) {
        if (!nsk_printer_match(nsk_header_table[i].shortcut)) {
            continue;
        }

        nsk_inf(",");

        const void *field = (char *)data + nsk_header_table[i].shift;
        const char *value = nsk_header_table[i].conv_short(field);

        if (strcmp(value, NSK_TABLE_NAN) == 0) {
            value = "";
        }
        nsk_inf("%s", value);
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
 * \brief  Prints the ROMs information as a CSV document
 */
void nsk_printer_csv(void) {
    _print_header();
    _print_entries();
}