#include <string.h>

#include "../../queue/printers/nsk_printer_table.h"

#include "../../types/header/nsk_header_table.h"
#include "../../queue/printers/nsk_printer_match.h"
#include "../../utils/log/nsk_log_inf.h"
#include "../../utils/nsk_util_max.h"
#include "../../types/header/nsk_header_data.h"
#include "../../utils/io/nsk_io_path.h"
#include "../../types/entry/nsk_entry_storage.h"
#include "../../utils/strings/nsk_strings_columns.h"

/*!
 * \brief  The size of the filename printed
 */
#define NSK_FILENAME_SLICESIZE (40)

/*!
 * \brief  Prints the table header
 */
static void _print_header(void) {
    nsk_inf("| %-*s |", NSK_FILENAME_SLICESIZE, "filename");

    for (size_t i = 0; i < nsk_header_tablesize; i++) {
        if (!nsk_printer_match(nsk_header_table[i].shortcut)) {
            continue;
        }

        size_t caption_len = strlen(nsk_header_table[i].shortcut);
        size_t field_len   = NSK_MAX(caption_len, nsk_header_table[i].length);

        nsk_inf(" %*s |", (int)field_len, nsk_header_table[i].shortcut);
    }

    nsk_inf("\n");
}

/*!
 * \brief  Prints the table splitter
 */
static void _print_split(void) {
    char splitter[128] = { 0 };
    memset(splitter, '-', sizeof(splitter) - 1);

    nsk_inf("|%.*s|", NSK_FILENAME_SLICESIZE + 2, splitter);

    for (size_t i = 0; i < nsk_header_tablesize; i++) {
        if (!nsk_printer_match(nsk_header_table[i].shortcut)) {
            continue;
        }

        size_t caption_len = strlen(nsk_header_table[i].shortcut);
        size_t field_len   = NSK_MAX(caption_len, nsk_header_table[i].length);

        nsk_inf("%.*s|", (int)field_len + 2, splitter);
    }

    nsk_inf("\n");
}

/*!
 * \brief  Returns either the passed argument or basename of it
 *
 * \param[in] filename  The filename
 * \return Shifted string
 */
static const char *_print_filename(const char *filename) {
    const char *basename = strrchr(filename, NSK_IO_SEP);
    return basename ? basename + 1 : filename;
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
    const char *basename = _print_filename(filename);
    size_t used;
    size_t bytes = nsk_strings_columns(basename, NSK_FILENAME_SLICESIZE, &used);
    size_t pad   =
        (used < NSK_FILENAME_SLICESIZE) ?
        (NSK_FILENAME_SLICESIZE - used) :
        0;

    nsk_inf(
        "| %.*s%*s |",
        (int)bytes,
        _print_filename(filename),
        (int)pad,
        ""
    );

    for (size_t i = 0; i < nsk_header_tablesize; i++) {
        if (!nsk_printer_match(nsk_header_table[i].shortcut)) {
            continue;
        }

        size_t caption_len = strlen(nsk_header_table[i].shortcut);
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
    _print_header();
    _print_split();
    _print_rows();
}
