#ifndef NSK_PATTABLES_TILES
#define NSK_PATTABLES_TILES

#include "../../types/nsk_type_pattable.h"
#include "../../types/nsk_type_palettes.h"
#include "../../types/pair/nsk_pair_type.h"

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
);

/*!
 * \brief  Assigns the tiles indexes to the pattern table
 *
 * \param[in] table     The table
 * \param[in] palette   The palette
 */
void nsk_pattable_settilesindexes(
    struct nsk_type_pattable        *table,
    const struct nsk_type_palette   *palette
);

/*!
 * \brief  Assigns the tiles colors to the pattern table
 *
 * \param[in] table     The table
 * \param[in] palette   The palette
 */
void nsk_pattable_settilescolors(
    struct nsk_type_pattable        *table,
    const struct nsk_type_palette   *palette
);

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
);

/*!
 * \brief  Assigns the tiles indexes to the pattern tables
 *
 * \param[in] tables    The tables
 * \param[in] palettes  The palettes
 */
void nsk_pattables_settilesindexes(
    struct nsk_type_pattables       *tables,
    const struct nsk_type_palettes  *palettes
);

/*!
 * \brief  Assigns the tiles colors to the pattern tables
 *
 * \param[in] tables    The tables
 * \param[in] palettes  The palettes
 */
void nsk_pattables_settilescolors(
    struct nsk_type_pattables       *tables,
    const struct nsk_type_palettes  *palettes
);

#endif