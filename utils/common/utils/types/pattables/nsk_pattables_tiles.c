#include <stdlib.h>

#include "../../types/pattables/nsk_pattables_tiles.h"
#include "../../types/nsk_type_tile.h"
#include "../../log/nsk_log_err.h"

/*!
 * \brief  Assigns the tiles palettes to the pattern table
 *
 * \param[in] table     The table
 * \param[in] palette   The palette
 */
void nsk_pattable_settilespalettes(
    struct nsk_type_pattable        *table,
    const struct nsk_type_palette   *palette
) {
    for (size_t h = 0; h < NSK_PATTABLETABLE_HEIGHT; h++) {
        for (size_t w = 0; w < NSK_PATTABLETABLE_WIDTH; w++) {
            if (!nsk_tile_setpalette(&table->tile[h][w], palette)) {
                nsk_err(
                    "Error at tile [$%zx%zx] of the %s table ($%04u)\n",
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
 */
void nsk_pattables_settilespalettes(
    struct nsk_type_pattables       *tables,
    const struct nsk_type_palettes  *palettes
) {
    for (size_t p = 0; p < NSK_PLANES_COUNT; p++) {
        nsk_pattable_settilespalettes(
            &tables->plane[p],
            &palettes->plane[p]
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
