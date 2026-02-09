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
 * \brief  Sets the tile palette by explicitly provided index
 *
 * \param     tile       The tile
 * \param[in] count      Number of colors used in the tile
 * \param[in] colors     The colors
 * \param[in] palette    The palette
 * \param[in] explicit   The explicitly requested palette index
 *
 * \return True if the palette is applied
 */
static bool _setpalette_explicitly(
    struct nsk_type_tile          *tile,
    size_t                         count,
    const union nsk_type_color4   *colors,
    const struct nsk_type_palette *palette,
    ssize_t                        explicit
) {
    for (size_t i = 0; i < count; i++) {
        size_t c;
        for (c = 0; c < NSK_PALETTESIZE_COLORS; c++) {
            if (palette->group[explicit].color[c].raw == colors[i].raw) {
                break;
            }
        }
        if (c == NSK_PALETTESIZE_COLORS) {
            nsk_err(
                "Requested tile palette #%zd does not contain the tile color %s\n",
                explicit,
                nsk_string_color(colors[i].r, colors[i].g, colors[i].b)
            );
            return false;
        }
    }

    tile->palette = explicit;
    return true;
}

/*!
 * \brief  Checks if both palettes has the same indexes for the tile
 * colors
 *
 * \param[in] count   The count of tile colors
 * \param[in] colors  The tile colors
 * \param[in] cand0   The candidate 0
 * \param[in] cand1   The candidate 1
 * \return True if all the colors has the same positions
 */
static bool _palettes_match(
    size_t                         count,
    const union nsk_type_color4   *colors,
    const union nsk_type_color4   *cand0,
    const union nsk_type_color4   *cand1
) {
    for (size_t i = 0; i < count; i++) {
        for (size_t c = 0; c < NSK_PALETTESIZE_COLORS; c++) {
            if (colors[i].raw == cand0[c].raw) {
                if (cand0[c].raw != cand1[c].raw) {
                    return false;
                }
                break;
            }
        }
    }
    return true;
}

/*!
 * \brief  Pretty prints the color list
 *
 * \param[in] name    The name of the list
 * \param[in] count   The number of colors
 * \param[in] colors  The color list
 */
static void _palette_print(
    const char                    *name,
    size_t                         count,
    const union nsk_type_color4   *colors
) {
    nsk_err("   - %s:\n", name);
    for (size_t i = 0; i < count; i++) {
        nsk_err(
            "%s%s%s",
            i == 0 ? "        " : ", ",
            nsk_string_color(
                colors[i].r,
                colors[i].g,
                colors[i].b
            ),
            i == count - 1 ? "\n" : ""
        );
    }
}

/*!
 * \brief  Sets the tile palette by heuristic analysis
 *
 * \param     tile       The tile
 * \param[in] count      Number of colors used in the tile
 * \param[in] colors     The colors
 * \param[in] palette    The palette
 * \param[in] explicit   The explicitly requested palette index
 *
 * \return True if the palette is applied
 */
static bool _setpalette_heuristically(
    struct nsk_type_tile          *tile,
    size_t                         count,
    const union nsk_type_color4   *colors,
    const struct nsk_type_palette *palette
) {
    bool selected = false;
    for (size_t g = 0; g < NSK_PALETTESIZE_GROUPS; g++) {
        bool match = true;

        for (size_t i = 0; i < count; i++) {
            size_t c;
            for (c = 0; c < NSK_PALETTESIZE_COLORS; c++) {
                if (palette->group[g].color[c].raw == colors[i].raw) {
                    break;
                }
            }

            if (c == NSK_PALETTESIZE_COLORS) {
                match = false;
                break;
            }
        }

        if (match) {
            if (selected) {
                bool ret = _palettes_match(
                    count,
                    colors,
                    palette->group[tile->palette].color,
                    palette->group[g].color
                );

                if (ret) {
                    continue;

                } else {
                    nsk_err(
                        "Two matching palettes have different tile colors positions:\n"
                    );
                    _palette_print(
                        "Candidate 1",
                        NSK_PALETTESIZE_COLORS,
                        palette->group[tile->palette].color
                    );
                    _palette_print(
                        "Candidate 2",
                        NSK_PALETTESIZE_COLORS,
                        palette->group[g].color
                    );
                    _palette_print(
                        "Tile colors",
                        count,
                        colors
                    );
                    nsk_err(
                        "\n"
                        "Suggested solutions:\n"
                        "   - Change the tile or tile colors to match exactly one palette\n"
                        "   - Provide \"--explicit-palettes\" option to explicitly set\n"
                        "     the tile's palette index\n"
                        "\n"
                    );
                    return false;
                }
            }

            tile->palette = g;
            selected = true;
        }
    }

    if (!selected) {
        nsk_err(
            "Cannot find matching palette for the tile\n"
        );
        nsk_err("   - Tile colors:\n");
        for (size_t i = 0; i < count; i++) {
            nsk_err(
                "%s%s%s",
                i == 0 ? "        " : ", ",
                nsk_string_color(
                    colors[i].r,
                    colors[i].g,
                    colors[i].b
                ),
                i == count - 1 ? "\n" : ""
            );
        }
        return false;
    }

    return true;
}

/*!
 * \brief  Sets the tile palette by colors
 *
 * \param[in,out] tile     The tile
 * \param[in]     palette  The palette
 * \param[in]     explicit The explicitly requested palette index or -1
 */
bool nsk_tile_setpalette(
    struct nsk_type_tile          *tile,
    const struct nsk_type_palette *palette,
    ssize_t                        explicit
) {
    size_t count;
    union nsk_type_color4 *list = nsk_tile_getcolors(tile, &count);
    bool result;

    if (explicit != -1) {
        result = _setpalette_explicitly(
            tile,
            count,
            list,
            palette,
            explicit
        );

    } else {
        result = _setpalette_heuristically(
            tile,
            count,
            list,
            palette
        );
    }

    tile->init.palette = true;

    nsk_util_free(list);

    return result;
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