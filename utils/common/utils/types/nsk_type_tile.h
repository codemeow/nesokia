#ifndef NSK_TYPE_TILE
#define NSK_TYPE_TILE

#if defined(NSK_MODULE_COMPOSE)

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "../types/nsk_type_color4.h"
#include "../types/nsk_type_palettes.h"

/*!
 * \brief  Tile sizes configuration
 */
enum nsk_tile_size {
    NSK_TILESIZE_WIDTH  = 8, /*!< Width in pixels  */
    NSK_TILESIZE_HEIGHT = 8, /*!< Height in pixels */
};

/*!
 * \brief  Single tile
 */
struct nsk_type_tile {
    size_t palette;               /*!< Tile palette (0..3)              */
    struct {
        union nsk_type_color4 color; /*!< Tile pixels data              */
        size_t index;                /*!< Color index in the palette    */
    } pixel
        [NSK_TILESIZE_WIDTH]
        [NSK_TILESIZE_HEIGHT];

    /*!
     * \brief List of initialized fields
     */
    struct {
        bool palette; /*!< Palette index is initialized  */
        bool colors;  /*!< Color values are initialized  */
        bool index;   /*!< Color indexes are initialized */
    } init;
};

/*!
 * \brief  Validates tile's palette field
 *
 * \param[in]  table  The table
 */
void nsk_tile_validate_palette(const struct nsk_type_tile *tile);

/*!
 * \brief  Validates tile's index field
 *
 * \param[in]  table  The table
 */
void nsk_tile_validate_index(const struct nsk_type_tile *tile);

/*!
 * \brief  Validates tile's colors field
 *
 * \param[in]  table  The table
 */
void nsk_tile_validate_colors(const struct nsk_type_tile *tile);

/*!
 * \brief  Checks if the selected tile is empty (filled with 0th color)
 *
 * \param[in] tile     The tile
 * \return True if the tile is filled with 0th color
 */
bool nsk_tile_isempty(
    const struct nsk_type_tile *tile
);

/*!
 * \brief  Populates the index of the color by the palette index and the color
 *
 * \param[in,out]  tile  The tile
 * \param[in]  palette Palette data
 */
void nsk_tile_setindex(
    struct nsk_type_tile *tile,
    const struct nsk_type_palette *palette
);

/*!
 * \brief  Populates the color data by the palette index and color index
 *
 * \param[in]  tile  The tile
 * \param[in]  palette Palette data
 */
void nsk_tile_setcolors(
    struct nsk_type_tile *tile,
    const struct nsk_type_palette *palette
);

/*!
 * \brief  Returns the list of used colors in the selected tile
 *
 * \param[in]  tile   The tile
 * \param[out] count  The colors count
 * \return Allocated list of colors
 */
union nsk_type_color4 *nsk_tile_getcolors(
    const struct nsk_type_tile *tile,
    size_t *count
) __attribute__((nonnull(2)));

/*!
 * \brief  Sets the tile palette by colors
 *
 * \param[in,out] tile     The tile
 * \param[in]     palette  The palette
 */
void nsk_tile_setpalette(
    struct nsk_type_tile *tile,
    const struct nsk_type_palette *palette
);

/*!
 * \brief  Reads single tile from the .chr/.chrs file
 *
 * \param[in,out]  tile      The tile
 * \param[in,out]  file      The file
 * \param[in]      filename  The filename
 */
void nsk_tile_readchr(
    struct nsk_type_tile *tile,
    FILE                 *file,
    const char           *filename
);

/*!
 * \brief  Writes one single tile to .chr/.chrs file
 *
 * \param[in]     tile      The tile
 * \param[in,out] file      The file
 * \param[in]     filename  The filename
 */
void nsk_tile_savechr(
    const struct nsk_type_tile *tile,
    FILE                       *file,
    const char                 *filename
);

#endif

#endif