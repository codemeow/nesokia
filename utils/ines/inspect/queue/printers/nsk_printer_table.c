#include <string.h>
#include <nsk_util_meta.h>

#include "../../queue/printers/nsk_printer_table.h"
#include "../../queue/printers/nsk_printer_match.h"
#include "../../types/entry/nsk_entry_storage.h"
#include "../../types/header/nsk_header_data.h"
#include "../../types/header/nsk_header_table.h"

/*!
 * The longest filename
 */
static size_t filename_maxlen = 0;

/*!
 * \brief  Precalculations for printing
 */
static void _print_prerequisites(void) {
    struct nsk_entry *entry = nsk_entry_storage;
    while (entry) {
        size_t len = nsk_strings_colums(entry->filename);

        if (len > filename_maxlen) {
            filename_maxlen = len;
        }

        entry = nsk_entry_next(entry);
    }
}

/*!
 * \brief  Prints the table header
 */
static void _print_header(void) {
    nsk_inf("| %-*s |", (int)filename_maxlen, "Filename");

    for (size_t i = 0; i < nsk_header_tablesize; i++) {
        if (!nsk_printer_match(nsk_header_table[i].shortcut)) {
            continue;
        }

        size_t caption_len = strlen(nsk_header_table[i].description);
        size_t field_len   = NSK_MAX(caption_len, nsk_header_table[i].length);

        nsk_inf(" %*s |", (int)field_len, nsk_header_table[i].description);
    }

    nsk_inf("\n");
}

/*!
 * \brief  Prints a horizontal table separator
 *
 * \param[in] size  The separator size
 */
static void _print_splitter(size_t size) {
    for (size_t i = 0; i < size; i++) {
        nsk_inf("-");
    }
}

/*!
 * \brief  Prints the table splitter
 */
static void _print_split(void) {
    nsk_inf("|");
    _print_splitter(filename_maxlen + 2);
    nsk_inf("|");

    for (size_t i = 0; i < nsk_header_tablesize; i++) {
        if (!nsk_printer_match(nsk_header_table[i].shortcut)) {
            continue;
        }

        size_t caption_len = strlen(nsk_header_table[i].description);
        size_t field_len   = NSK_MAX(caption_len, nsk_header_table[i].length);

        _print_splitter(field_len + 2);
        nsk_inf("|");
    }

    nsk_inf("\n");
}

/*!
 * \brief  Prints single row
 *
 * \param[in] filename  The filename
 * \param[in] data      Parsed header data
 */
static void _print_row(
    const char *filename,
    const struct nsk_header_data *data
) {
    size_t used;
    size_t bytes = nsk_strings_limitcolumns(filename, filename_maxlen, &used);
    size_t pad   =
        (used < filename_maxlen) ?
        (filename_maxlen - used) :
        0;

    nsk_inf(
        "| %.*s%*s |",
        (int)bytes,
        filename,
        (int)pad,
        ""
    );

    for (size_t i = 0; i < nsk_header_tablesize; i++) {
        if (!nsk_printer_match(nsk_header_table[i].shortcut)) {
            continue;
        }

        size_t caption_len = strlen(nsk_header_table[i].description);
        size_t field_len   = NSK_MAX(caption_len, nsk_header_table[i].length);

        const void *field = (char *)data + nsk_header_table[i].shift;
        nsk_inf(
            " %*s |",
            (int)field_len,
            nsk_header_table[i].conv_short(field)
        );
    }

    nsk_inf("\n");
}

/*!
 * \brief  Prints all rows
 */
static void _print_rows(void) {
    struct nsk_entry *entry = nsk_entry_storage;
    while (entry) {
        _print_row(entry->filename, &entry->data);

        entry = nsk_entry_next(entry);
    }
}

/*!
 * \brief  Prints the ROM information as a table view
 */
void nsk_printer_table(void) {
    _print_prerequisites();
    _print_header();
    _print_split();
    _print_rows();
}
