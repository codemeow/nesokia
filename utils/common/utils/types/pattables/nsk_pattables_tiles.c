#include <stdlib.h>
#include <string.h>

#include "../../types/pattables/nsk_pattables_tiles.h"
#include "../../types/nsk_type_tile.h"
#include "../../log/nsk_log_err.h"

/*!
 * \brief  Finds the tile's requested explicit palette in the list
 *
 * \param[in] explicit   The list of explicitly requested palettes
 * \param[in] address    The pattern table address
 * \param[in] x          The tile X index
 * \param[in] y          The tile Y index
 * \return Index of the requested palette or -1
 */
static ssize_t _explicit_find(
    const struct nsk_pair           *explicit,
    enum nsk_pattable_address        address,
    size_t                           x,
    size_t                           y
) {
    if (!explicit) {
        return -1;
    }

    char name[8];
    snprintf(
        name,
        sizeof(name),
        "%c%zx%zx",
        address == NSK_PATTABLEADDR_0000 ? 'l' : 'r',
        y,
        x
    );

    const struct nsk_pair *pair = explicit;
    while (pair) {
        if (strcmp(pair->name, name) == 0) {
            return pair->value;
        }

        pair = nsk_pair_next(pair);
    }

    return -1;
}

/*!
 * \brief  Assigns the tiles palettes to the pattern table
 *
 * \param[in] table     The table
 * \param[in] palette   The palette
 * \param[in] explicit  The list of explici palette indexes
 */
void nsk_pattable_settilespalettes(
    struct nsk_type_pattable        *table,
    const struct nsk_type_palette   *palette,
    const struct nsk_pair           *explicit
) {

    for (size_t h = 0; h < NSK_PATTABLETABLE_HEIGHT; h++) {
        for (size_t w = 0; w < NSK_PATTABLETABLE_WIDTH; w++) {
            bool ret = nsk_tile_setpalette(
                &table->tile[h][w],
                palette,
                _explicit_find(explicit, table->address, w, h)
            );
            if (!ret) {
                nsk_err(
                    "Error at tile [$%zx%zx] of the %s table ($%04x)\n",
                    h,
                    w,
                    nsk_conv_plane2string(table->plane),
                    nsk_conv_pataddress2value(table->address)
                );
                exit(EXIT_FAILURE);
            }
        }
    }
}

/*!
 * \brief  Assigns the tiles indexes to the pattern table
 *
 * \param[in] table     The table
 * \param[in] palette   The palette
 */
void nsk_pattable_settilesindexes(
    struct nsk_type_pattable        *table,
    const struct nsk_type_palette   *palette
) {
    for (size_t h = 0; h < NSK_PATTABLETABLE_HEIGHT; h++) {
        for (size_t w = 0; w < NSK_PATTABLETABLE_WIDTH; w++) {
            nsk_tile_setindex(&table->tile[h][w], palette);
        }
    }
}

/*!
 * \brief  Assigns the tiles colors to the pattern table
 *
 * \param[in] table     The table
 * \param[in] palette   The palette
 */
void nsk_pattable_settilescolors(
    struct nsk_type_pattable        *table,
    const struct nsk_type_palette   *palette
) {
    for (size_t h = 0; h < NSK_PATTABLETABLE_HEIGHT; h++) {
        for (size_t w = 0; w < NSK_PATTABLETABLE_WIDTH; w++) {
            nsk_tile_setcolors(&table->tile[h][w], palette);
        }
    }
}

/*!
 * \brief  Assigns the tiles palettes to the pattern tables
 *
 * \param[in] tables    The tables
 * \param[in] palettes  The palettes
 * \param[in] explicit  The list of explici palette indexes
 */
void nsk_pattables_settilespalettes(
    struct nsk_type_pattables       *tables,
    const struct nsk_type_palettes  *palettes,
    const struct nsk_pair           *explicit
) {
    for (size_t p = 0; p < NSK_PLANES_COUNT; p++) {
        nsk_pattable_settilespalettes(
            &tables->plane[p],
            &palettes->plane[p],
            explicit
        );
    }
}

/*!
 * \brief  Assigns the tiles indexes to the pattern tables
 *
 * \param[in] tables    The tables
 * \param[in] palettes  The palettes
 */
void nsk_pattables_settilesindexes(
    struct nsk_type_pattables       *tables,
    const struct nsk_type_palettes  *palettes
) {
    for (size_t p = 0; p < NSK_PLANES_COUNT; p++) {
        nsk_pattable_settilesindexes(
            &tables->plane[p],
            &palettes->plane[p]
        );
    }
}

/*!
 * \brief  Assigns the tiles colors to the pattern tables
 *
 * \param[in] tables    The tables
 * \param[in] palettes  The palettes
 */
void nsk_pattables_settilescolors(
    struct nsk_type_pattables       *tables,
    const struct nsk_type_palettes  *palettes
) {
    for (size_t p = 0; p < NSK_PLANES_COUNT; p++) {
        nsk_pattable_settilescolors(
            &tables->plane[p],
            &palettes->plane[p]
        );
    }
}
