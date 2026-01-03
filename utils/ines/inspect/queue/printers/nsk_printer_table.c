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
 * \brief  Returns either the passed argument or basename of it
 *
 * \param[in] filename  The filename
 * \return Shifted string
 */
static const char *_print_filename(const char *filename) {
    const char *basename = strrchr(filename, nsk_path_delimeter);
    return basename ? basename + 1 : filename;
}

/*!
 * \brief  Precalculations for printing
 */
static void _print_prerequisites(void) {
    struct nsk_entry *entry = nsk_entry_storage;
    while (entry) {
        const char *basename = _print_filename(entry->filename);
        size_t len = nsk_strings_colums(basename);

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
 * \brief  Prints the table splitter
 */
static void _print_split(void) {
    char splitter[128] = { 0 };
    memset(splitter, '-', sizeof(splitter) - 1);

    nsk_inf("|%.*s|", (int)filename_maxlen + 2, splitter);

    for (size_t i = 0; i < nsk_header_tablesize; i++) {
        if (!nsk_printer_match(nsk_header_table[i].shortcut)) {
            continue;
        }

        size_t caption_len = strlen(nsk_header_table[i].description);
        size_t field_len   = NSK_MAX(caption_len, nsk_header_table[i].length);

        nsk_inf("%.*s|", (int)field_len + 2, splitter);
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
    const char *basename = _print_filename(filename);
    size_t used;
    size_t bytes = nsk_strings_limitcolumns(basename, filename_maxlen, &used);
    size_t pad   =
        (used < filename_maxlen) ?
        (filename_maxlen - used) :
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
