#include <limits.h>
#include <errno.h>
#include <stdlib.h>

#include "../types/nsk_type_pattable.h"
#include "../log/nsk_log_err.h"
#include "../utils/nsk_util_cleanup.h"
#include "../utils/io/nsk_io_fopen.h"
#include "../error/nsk_util_errno.h"
#include "../utils/log/nsk_log_inf.h"

/*!
 * \brief  Converts the enum value to the raw value
 *
 * \param[in] address  The address
 * \return Raw address value
 */
unsigned nsk_conv_chraddress2value(enum nsk_pattable_address address) {
    switch (address) {
        case NSK_PATTABLEADDR_0000: return 0x0000;
        case NSK_PATTABLEADDR_1000: return 0x1000;

        default:
            nsk_err(
                "Error: Unknown address value: %d\n",
                (int)address
            );
            abort();
    }
}

/*!
 * \brief  Generic validator
 *
 * \param[in] flag     The flag
 * \param[in] message  The message to write if the flag is false
 */
static void _pattable_validate(
    bool flag,
    const char *message
) {
    if (!flag) {
        nsk_err(
            "Error: Pattern table field \"%s\"is not initialized\n",
            message
        );
        abort();
    }
}

/*!
 * \brief  Validates pattern table's plane field
 *
 * \param[in]  table  The table
 */
void nsk_pattable_validate_plane(const struct nsk_type_pattable *table) {
    _pattable_validate(table->init.plane, "plane");
}

/*!
 * \brief  Validates pattern table's address field
 *
 * \param[in]  table  The table
 */
void nsk_pattable_validate_address(const struct nsk_type_pattable *table) {
    _pattable_validate(table->init.address, "address");
}

/*!
 * \brief  Reads single pattern table from .chr/.chrs file
 *
 * \param[in]  file      The file
 * \param[in]  filename  The filename
 * \param[out] table     The table
 */
static void _pattable_readpat(
    FILE                        *file,
    const char                  *filename,
    struct nsk_type_pattable    *table
) {
    for (size_t h = 0; h < NSK_PATTABLETABLE_HEIGHT; h++) {
        for (size_t w = 0; w < NSK_PATTABLETABLE_WIDTH; w++) {
            nsk_tile_readchr(
                &table->tile[h][w],
                file,
                filename
            );
        }
    }
}

/*!
 * \brief  Saves single pattern table to file
 *
 * \param[in] file      The file
 * \param[in] filename  The filename
 * \param[in] table     The table
 */
static void _pattable_savepat(
    FILE *file,
    const char *filename,
    const struct nsk_type_pattable *table
) {
    for (size_t h = 0; h < NSK_PATTABLETABLE_HEIGHT; h++) {
        for (size_t w = 0; w < NSK_PATTABLETABLE_WIDTH; w++) {
            nsk_tile_savechr(
                &table->tile[h][w],
                file,
                filename
            );
        }
    }
}

/*!
 * \brief  Reads pattern table from .chr file
 * (binary $0000..$0fff/$1000..$1fff)
 *
 * \param[in] filename  The filename
 * \return The resulting pattern table
 */
struct nsk_type_pattable nsk_pattable_readpat(
    const char *filename
) {
    struct nsk_type_pattable table = { 0 };
    nsk_auto_fclose FILE *file = nsk_io_fopen(filename, "rb");
    if (!file) {
        nsk_err(
            "Error: cannot open file \"%s\" for pattern table reading.\n"
            "Possible reason: %s\n",
            filename,
            nsk_util_strerror(errno)
        );
        exit(EXIT_FAILURE);
    }

    _pattable_readpat(file, filename, &table);

    return table;
}

/*!
 * \brief  Reads pattern tables from .chrs file
 * (binary $0000..$1fff)
 *
 * \param[in] filename  The filename
 * \return The resulting pattern tables
 */
struct nsk_type_pattables nsk_pattables_readpats(
    const char *filename
) {
    struct nsk_type_pattables tables = { 0 };
    nsk_auto_fclose FILE *file = nsk_io_fopen(filename, "rb");
    if (!file) {
        nsk_err(
            "Error: cannot open file \"%s\" for pattern tables reading.\n"
            "Possible reason: %s\n",
            filename,
            nsk_util_strerror(errno)
        );
        exit(EXIT_FAILURE);
    }

    for (size_t p = 0; p < NSK_PLANES_COUNT; p++) {
        _pattable_readpat(file, filename, &tables.plane[p]);
    }

    return tables;
}

/*!
 * \brief  Saves selected pattern table as .chr file
 *
 * \note The palette indexes should be applied first with
 * nsk_pattable_apply
 *
 * \param[in] filename  The filename
 * \param[in] table     The table
 */
void nsk_pattable_savepat(
    const char *filename,
    const struct nsk_type_pattable *table
) {
    nsk_auto_fclose FILE *file = nsk_io_fopen(filename, "wb");
    if (!file) {
        nsk_err(
            "Error: cannot open file \"%s\" for pattern table writing.\n"
            "Possible reason: %s\n",
            filename,
            nsk_util_strerror(errno)
        );
        exit(EXIT_FAILURE);
    }

    _pattable_savepat(file, filename, table);
}

/*!
 * \brief  Saves pattern tables as .chrs file
 *
 * \note The palette indexes should be applied first with
 * nsk_pattable_apply
 *
 * \param[in] filename  The filename
 * \param[in] tables    The tables
 */
void nsk_pattables_savepats(
    const char *filename,
    const struct nsk_type_pattables *tables
) {
    nsk_auto_fclose FILE *file = nsk_io_fopen(filename, "wb");
    if (!file) {
        nsk_err(
            "Error: cannot open file \"%s\" for pattern tables writing.\n"
            "Possible reason: %s\n",
            filename,
            nsk_util_strerror(errno)
        );
        exit(EXIT_FAILURE);
    }

    for (size_t p = 0; p < NSK_PLANES_COUNT; p++) {
        _pattable_savepat(file, filename, &tables->plane[p]);
    }
}

/*!
 * \brief  Shows selected pattern table as ANSI colored output
 *
 * \param[in] table  The pattern table
 */
void nsk_pattable_show(
    const struct nsk_type_pattable *table
) {
    nsk_pattable_validate_plane  (table);
    nsk_pattable_validate_address(table);

    nsk_inf(
        "# %s pattern table ($%04x)\n",
        nsk_conv_plane2string(table->plane),
        nsk_conv_chraddress2value(table->address)
    );

    nsk_inf("        ");
    for (size_t w = 0; w < NSK_PATTABLETABLE_WIDTH; w++) {
        nsk_inf("-%zx ", w);
    }
    nsk_inf("\n");

    for (size_t ty = 0; ty < NSK_PATTABLETABLE_HEIGHT; ty++) {
        nsk_inf(
            "  $%03zx- ",
            nsk_conv_chraddress2value(table->address) / 0x10 + ty
        );

        for (size_t tx = 0; tx < NSK_PATTABLETABLE_WIDTH; tx++) {
            bool empty = nsk_tile_isempty(
                &table->tile[tx][ty]
            );

            if (empty) {
                nsk_inf(".. ");
            } else {
                nsk_inf("## ");
            }
        }
        nsk_inf("\n");
    }

    nsk_inf("\n");
}

/*!
 * \brief  Shows pattern tables as ANSI colored output
 *
 * \param[in] tables  The pattern tables
 */
void nsk_pattables_show(
    const struct nsk_type_pattables *tables
) {
    for (size_t p = 0; p < NSK_PLANES_COUNT; p++) {
        nsk_pattable_show(
            &tables->plane[p]
        );
    }
}

/*!
 * \brief  Assigns the tiles properties to the selected pattern table
 *
 * \param[in] palette  The palette
 * \param[in] table    The table
 */
void nsk_pattable_settiles(
    const struct nsk_type_palette  *palette,
    struct nsk_type_pattable *table
) {
    for (size_t h = 0; h < NSK_PATTABLETABLE_HEIGHT; h++) {
        for (size_t w = 0; w < NSK_PATTABLETABLE_WIDTH; w++) {
            nsk_tile_setpalette(&table->tile[h][w], palette);
            nsk_tile_setindex  (&table->tile[h][w], palette);
        }
    }
}

/*!
 * \brief  Assigns the tiles properties to the patter tables
 *
 * \param[in] palettes  The palettes
 * \param[in] tables    The tables
 */
void nsk_pattables_settiles(
    const struct nsk_type_palettes  *palettes,
    struct nsk_type_pattables *tables
) {
    for (size_t p = 0; p < NSK_PLANES_COUNT; p++) {
        nsk_pattable_settiles(
            &palettes->plane[p],
            &tables->plane[p]
        );
    }
}

/*!
 * \brief  By address accessor
 *
 * \param[in] tables   The tables
 * \param[in] address  The address
 * \return Matching pattern table
 */
const struct nsk_type_pattable *nsk_pattables_getbyaddress(
    const struct nsk_type_pattables *tables,
    enum nsk_pattable_address address
) {
    for (size_t p = 0; p < NSK_PLANES_COUNT; p++) {
        if (tables->plane[p].address == address) {
            return &tables->plane[p];
        }
    }

    nsk_err(
        "Matching pattern table not found (by address %d)\n",
        (int)address
    );
    abort();
}


/*!
 * \brief  Sets the plane field and marks the init section
 *
 * \param[in,out]  table  The table
 * \param[in]      plane  The plane value
 */
void nsk_pattable_setplane(
    struct nsk_type_pattable *table,
    enum nsk_plane_list plane
) {
    table->plane = plane;
    table->init.plane = true;
}

/*!
 * \brief  Sets the address field and marks the init section
 *
 * \param[in,out]  table   The table
 * \param[in]      address The address value
 */
void nsk_pattable_setaddress(
    struct nsk_type_pattable *table,
    enum nsk_pattable_address address
) {
    table->address = address;
    table->init.address = true;
}