#include <stdlib.h>

#include "../types/nsk_type_tile.h"
#include "../log/nsk_log_err.h"
#include "../nsk_util_malloc.h"
#include "../strings/nsk_strings_ansi.h"

/*!
 * \brief  Generic validator
 *
 * \param[in] flag     The flag
 * \param[in] message  The message to write if the flag is false
 */
static void _tile_validate(
    bool flag,
    const char *message
) {
    if (!flag) {
        nsk_err(
            "Error: Tile field \"%s\" is not initialized\n",
            message
        );
        abort();
    }
}

/*!
 * \brief  Validates tile's palette field
 *
 * \param[in]  table  The table
 */
void nsk_tile_validate_palette(const struct nsk_type_tile *tile) {
    _tile_validate(tile->init.palette, "palette");
}

/*!
 * \brief  Validates tile's index field
 *
 * \param[in]  table  The table
 */
void nsk_tile_validate_index(const struct nsk_type_tile *tile) {
    _tile_validate(tile->init.index, "index");
}

/*!
 * \brief  Validates tile's colors field
 *
 * \param[in]  table  The table
 */
void nsk_tile_validate_colors(const struct nsk_type_tile *tile) {
    _tile_validate(tile->init.colors, "colors");
}

/*!
 * \brief  Checks if the selected tile is empty (filled with 0th color)
 *
 * \param[in] tile     The tile
 * \return True if the tile is filled with 0th color
 */
bool nsk_tile_isempty(
    const struct nsk_type_tile *tile
) {
    nsk_tile_validate_index(tile);

    for (size_t h = 0; h < NSK_TILESIZE_HEIGHT; h++) {
        for (size_t w = 0; w < NSK_TILESIZE_WIDTH; w++) {
            if (tile->pixel[h][w].index != 0) {
                return false;
            }
        }
    }

    return true;
}

/*!
 * \brief  Populates the index of the color by the palette index and the color
 *
 * \param[in,out]  tile  The tile
 * \param[in]  palette Palette data
 */
void nsk_tile_setindex(
    struct nsk_type_tile *tile,
    const struct nsk_type_palette *palette
) {
    nsk_tile_validate_palette(tile);
    nsk_tile_validate_colors(tile);

    for (size_t h = 0; h < NSK_TILESIZE_HEIGHT; h++) {
        for (size_t w = 0; w < NSK_TILESIZE_WIDTH; w++) {
            tile->pixel[h][w].index = nsk_palette_getindex(
                palette,
                tile->palette,
                &tile->pixel[h][w].color
            );
        }
    }

    tile->init.index = true;
}

/*!
 * \brief  Populates the color data by the palette index and color index
 *
 * \param[in]  tile  The tile
 * \param[in]  palette Palette data
 */
void nsk_tile_setcolors(
    struct nsk_type_tile *tile,
    const struct nsk_type_palette *palette
) {
    nsk_tile_validate_palette(tile);
    nsk_tile_validate_index(tile);

    for (size_t h = 0; h < NSK_TILESIZE_HEIGHT; h++) {
        for (size_t w = 0; w < NSK_TILESIZE_WIDTH; w++) {
            size_t index = tile->pixel[h][w].index;
            tile->pixel[h][w].color =
                palette->group[tile->palette].color[index];
        }
    }

    tile->init.colors = true;
}

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
) {
    *count = 1;
    union nsk_type_color4 *list = nsk_util_malloc(
        sizeof(*list) * *count
    );
    list[0] = tile->pixel[0][0].color;

    for (size_t h = 0; h < NSK_TILESIZE_HEIGHT; h++) {
        for (size_t w = 0; w < NSK_TILESIZE_WIDTH; w++) {
            const union nsk_type_color4 color = tile->pixel[h][w].color;

            size_t i;
            for (i = 0; i < *count; i++) {
                if (list[i].raw == color.raw) {
                    break;
                }
            }
            if (i == *count) {
                list = nsk_util_realloc(
                    list,
                    sizeof(*list) * (*count + 1)
                );
                list[*count] = color;
                *count = *count + 1;
            }
        }
    }
    return list;
}

/*!
 * \brief  Sets the tile palette by colors
 *
 * \param[in,out] tile     The tile
 * \param[in]     palette  The palette
 */
bool nsk_tile_setpalette(
    struct nsk_type_tile *tile,
    const struct nsk_type_palette *palette
) {
    size_t count;
    union nsk_type_color4 *list = nsk_tile_getcolors(tile, &count);

    size_t g;
    for (g = 0; g < NSK_PALETTESIZE_GROUPS; g++) {
        bool match = true;

        for (size_t i = 0; i < count; i++) {
            size_t c;
            for (c = 0; c < NSK_PALETTESIZE_COLORS; c++) {
                if (palette->group[g].color[c].raw == list[i].raw) {
                    break;
                }
            }

            if (c == NSK_PALETTESIZE_COLORS) {
                match = false;
                break;
            }
        }

        if (match) {
            tile->palette = g;
            break;
        }
    }

    if (g == NSK_PALETTESIZE_GROUPS) {
        nsk_err(
            "Cannot assign palette to the tile\n"
        );
        nsk_err(
            "Used colors:\n"
        );
        for (size_t i = 0; i < count; i++) {
            nsk_err(
                "  - %s\n",
                nsk_string_color(list[i].r, list[i].g, list[i].b)
            );
        }
        return false;
    }

    tile->init.palette = true;

    nsk_util_free(list);

    return true;
}

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
) {
    uint8_t planes[2][NSK_TILESIZE_HEIGHT] = { 0 };

    if (fread(planes, sizeof(planes), 1, file) != 1) {
        nsk_err(
            "Error: cannot read data from \"%s\" CHR file\n",
            filename
        );
        exit(EXIT_FAILURE);
    }

    for (size_t y = 0; y < NSK_TILESIZE_HEIGHT; y++) {
        for (size_t x = 0; x < NSK_TILESIZE_WIDTH; x++) {
            unsigned shift = NSK_TILESIZE_WIDTH - x - 1;

            uint8_t bit0 = (planes[0][y] >> shift) & 1;
            uint8_t bit1 = (planes[1][y] >> shift) & 1;

            tile->pixel[y][x].index = bit0 | (bit1 << 1);
        }
    }

    tile->init.index = true;
}

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
) {
    nsk_tile_validate_index(tile);

    uint8_t planes[2][NSK_TILESIZE_HEIGHT] = { 0 };


    for (size_t y = 0; y < NSK_TILESIZE_HEIGHT; y++) {
        for (size_t x = 0; x < NSK_TILESIZE_WIDTH; x++) {
            unsigned shift = NSK_TILESIZE_WIDTH - x - 1;

            planes[0][y] |= ((tile->pixel[y][x].index >> 0) & 1) << shift;
            planes[1][y] |= ((tile->pixel[y][x].index >> 1) & 1) << shift;
        }
    }

    if (fwrite(planes, sizeof(planes), 1, file) != 1) {
        nsk_err(
            "Error: cannot write data to \"%s\" CHR file\n",
            filename
        );
        exit(EXIT_FAILURE);
    }
}