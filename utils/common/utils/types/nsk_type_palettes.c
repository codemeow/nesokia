#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <threads.h>

#include "types/nsk_type_palettes.h"
#include "log/nsk_log_err.h"
#include "strings/nsk_strings_ansi.h"
#include "log/nsk_log_inf.h"
#include "io/nsk_io_fopen.h"
#include "error/nsk_util_errno.h"
#include "base/nsk_util_cleanup.h"
#include "base/nsk_util_size.h"

/*!
 * \brief  Number of static strings in functions, returning static strings
 */
#define _STATIC_CAROUSEL_SIZE (5)

/*!
 * \brief  Returns the binary string representation (00 .. 11)
 *
 * \param[in] value  The value
 * \return Static string
 */
static const char *_string_binary(size_t value) {
    static thread_local char string[_STATIC_CAROUSEL_SIZE][8];
    static thread_local size_t index;

    if (++index >= _STATIC_CAROUSEL_SIZE) {
        index = 0;
    }

    snprintf(
        string[index],
        sizeof(string[index]),
        "%zu%zu",
        (value & (1 << 1)) >> 1,
        (value & (1 << 0)) >> 0
    );
    return string[index];
}

/*!
 * \brief  Generic validator
 *
 * \param[in] flag     The flag
 * \param[in] message  The message to write if the flag is false
 */
static void _palette_validate(
    bool flag,
    const char *message
) {
    if (!flag) {
        nsk_err(
            "Error: Palette field \"%s\" is not initialized\n",
            message
        );
        abort();
    }
}

/*!
 * \brief  Validates palette's colors field
 *
 * \param[in]  palette  The palette
 *
 * \return True if the field is initialized, false otherwise
 */
bool nsk_palette_validate_colors(const struct nsk_type_palette *palette) {
    if (!palette->init.colors) {
        nsk_err(
            "Error: Palette field \"%s\" is not initialized\n",
            "colors"
        );
        return false;
    }

    return true;
}

/*!
 * \brief  Validates palette's indexes field
 *
 * \param[in]  palette  The palette
 *
 * \note This is a fatal workflow invariant check. It returns only when the
 *       field is initialized; otherwise it terminates the process.
 */
void nsk_palette_validate_indexes(const struct nsk_type_palette *palette) {
    _palette_validate(palette->init.indexes, "indexes");
}

/*!
 * \brief  Reads .spal format palette from the provided file
 *
 * \param[in]      file     The file
 * \param[in]      filename File's filename
 * \param[in,out]  palette  The palette
 * \return True if the palette was read, false otherwise
 */
static bool _palette_readspal(
    FILE *file,
    const char *filename,
    struct nsk_type_palette *palette
) {
    for (size_t g = 0; g < NSK_SIZE(palette->group); g++) {
        for (size_t c = 0; c < NSK_SIZE(palette->group[0].index); c++) {
            uint8_t index;
            if (fread(&index, sizeof(index), 1, file) != 1) {
                nsk_err(
                    "Error: cannot read file \"%s\" for palette reading.\n"
                    "Possible reason: %s\n",
                    filename,
                    nsk_util_strerror(errno)
                );
                return false;
            }
            palette->group[g].index[c] = index;
        }
    }

    palette->init.indexes = true;
    return true;
}

/*!
 * \brief  Saves .spal format palette to the provided file
 *
 * \param[in]       file      The file
 * \param[in]       filename  The filename
 * \param[in,out]   palette   The palette
 * \return True if the palette was saved, false otherwise
 */
static bool _palette_savespal(
    FILE *file,
    const char *filename,
    const struct nsk_type_palette *palette
) {
    nsk_palette_validate_indexes(palette);

    for (size_t g = 0; g < NSK_SIZE(palette->group); g++) {
        for (size_t c = 0; c < NSK_SIZE(palette->group[0].index); c++) {
            uint8_t index = palette->group[g].index[c];
            if (fwrite(&index, sizeof(index), 1, file) != 1) {
                nsk_err(
                    "Error: cannot write file \"%s\" for palette writing.\n"
                    "Possible reason: %s\n",
                    filename,
                    nsk_util_strerror(errno)
                );
                return false;
            }
        }
    }

    return true;
}

/*!
 * \brief  Reads .spal file ("Selected palette")
 * (binary $3f00..$3f0f/$3f10..$3f1f)
 *
 * \note Reads only `index` fields. Call `nsk_palette_apply` function
 *       to fill the actual colors
 *
 * \param[in]  filename  The filename
 * \param[out] palette   The palette
 * \return True if the palette was read, false otherwise
 */
bool nsk_palette_readspal(
    const char *filename,
    struct nsk_type_palette *palette
) {
    *palette = (struct nsk_type_palette){ 0 };

    nsk_auto_fclose FILE *file = nsk_io_fopen(filename, "rb");
    if (!file) {
        nsk_err(
            "Error: cannot open file \"%s\" for palette colors reading.\n"
            "Possible reason: %s\n",
            filename,
            nsk_util_strerror(errno)
        );
        return false;
    }

    return _palette_readspal(file, filename, palette);
}

/*!
 * \brief  Saves selected palette as binary .spal file
 *
 * \param[in] filename  The filename
 * \param[in] palette   The palette
 * \return True if the palette was saved, false otherwise
 */
bool nsk_palette_savespal(
    const char *filename,
    const struct nsk_type_palette *palette
) {
    nsk_auto_fclose FILE *file = nsk_io_fopen(filename, "wb");
    if (!file) {
        nsk_err(
            "Error: cannot open file \"%s\" for palette colors writing.\n"
            "Possible reason: %s\n",
            filename,
            nsk_util_strerror(errno)
        );
        return false;
    }

    return _palette_savespal(file, filename, palette);
}

/*!
 * \brief  Reads .spals file ("Selected palettes")
 *
 * \note Reads only `index` fields. Call `nsk_palette_apply` function
 *       to fill the actual colors
 * (binary $3f00..$3f1f)
 *
 * \param[in]  filename  The filename
 * \param[out] palettes  The palettes
 * \return True if the palettes were read, false otherwise
 */
bool nsk_palettes_readspals(
    const char *filename,
    struct nsk_type_palettes *palettes
) {
    *palettes = (struct nsk_type_palettes){ 0 };

    nsk_auto_fclose FILE *file = nsk_io_fopen(filename, "rb");
    if (!file) {
        nsk_err(
            "Error: cannot open file \"%s\" for palettes colors reading.\n"
            "Possible reason: %s\n",
            filename,
            nsk_util_strerror(errno)
        );
        return false;
    }

    for (size_t p = 0; p < NSK_SIZE(palettes->plane); p++) {
        if (!_palette_readspal(file, filename, &palettes->plane[p])) {
            return false;
        }
    }

    return true;
}

/*!
 * \brief  Saves both palettes as binary .spals file
 *
 * \param[in] filename  The filename
 * \param[in] palettes  The palettes
 * \return True if the palettes were saved, false otherwise
 */
bool nsk_palettes_savespals(
    const char *filename,
    const struct nsk_type_palettes *palettes
) {
    nsk_auto_fclose FILE *file = nsk_io_fopen(filename, "wb");
    if (!file) {
        nsk_err(
            "Error: cannot open file \"%s\" for palette colors writing.\n"
            "Possible reason: %s\n",
            filename,
            nsk_util_strerror(errno)
        );
        return false;
    }

    for (size_t p = 0; p < NSK_SIZE(palettes->plane); p++) {
        if (!_palette_savespal(file, filename, &palettes->plane[p])) {
            return false;
        }
    }

    return true;
}

/*!
 * \brief  Shows selected palette as ANSI colored output
 *
 * \param[in] palette  The palette
 * \return True if the palette was shown, false otherwise
 */
bool nsk_palette_show(
    const struct nsk_type_palette *palette
) {
    if (!nsk_palette_validate_colors(palette)) {
        return false;
    }

    nsk_inf(
        "# %s palette ($3f-)\n",
        nsk_conv_plane2string(palette->plane)
    );

    nsk_inf("           ");
    for (size_t c = 0; c < NSK_PALETTESIZE_COLORS; c++) {
        nsk_inf("   -%s ", _string_binary(c));
    }
    nsk_inf("\n");

    for (size_t g = 0; g < NSK_PALETTESIZE_GROUPS; g++) {
        nsk_inf("  %%%04d %s- ", (int)palette->plane, _string_binary(g));
        for (size_t c = 0; c < NSK_PALETTESIZE_COLORS; c++) {
            const union nsk_type_color4 *color = &palette->group[g].color[c];
            nsk_inf(
                "%s ",
                nsk_string_color(color->r, color->g, color->b)
            );
        }
        nsk_inf("\n");
    }
    nsk_inf("\n");

    return true;
}

/*!
 * \brief  Shows selected palettes as ANSI colored output
 *
 * \param[in] palettes  The palettes
 * \return True if the palettes were shown, false otherwise
 */
bool nsk_palettes_show(
    const struct nsk_type_palettes *palettes
) {
    for (size_t p = 0; p < NSK_PLANES_COUNT; p++) {
        if (!nsk_palette_show(&palettes->plane[p])) {
            return false;
        }
    }

    return true;
}

/*!
 * \brief  Validates the single palette
 *
 * \param[in] colors   The colors
 * \param[in] palette  The palette
 */
bool nsk_palette_validate(
    const struct nsk_type_ppucolors *colors,
    const struct nsk_type_palette   *palette
) {
    nsk_palette_validate_indexes(palette);

    for (size_t g = 0; g < NSK_PALETTESIZE_GROUPS; g++) {
        for (size_t c = 0; c < NSK_PALETTESIZE_COLORS; c++) {
            if (palette->group[g].index[c] >= NSK_PPUCOLORSTABLE_COUNT) {
                nsk_err(
                    "Error: invalid index for %s palette color %zu:%zu: %zu\n",
                    nsk_conv_plane2string(palette->plane),
                    g,
                    c,
                    palette->group[g].index[c]
                );
                return false;
            }

            if (!colors->allowed[palette->group[g].index[c]]) {
                nsk_err(
                    "Error: color for %s palette color %zu:%zu: not allowed\n",
                    nsk_conv_plane2string(palette->plane),
                    g,
                    c
                );
                return false;
            }
        }
    }

    return true;
}

/*!
 * \brief  Validates all palettes
 *
 * \param[in] colors   The colors
 * \param[in] palettes  The palettes
 */
bool nsk_palettes_validate(
    const struct nsk_type_ppucolors *colors,
    const struct nsk_type_palettes  *palettes
) {
    if (!nsk_palette_validate_colors(&palettes->plane[NSK_PLANE_BACKGROUND]) ||
        !nsk_palette_validate_colors(&palettes->plane[NSK_PLANE_SPRITES])) {
        return false;
    }

    for (size_t p = 0; p < NSK_PLANES_COUNT; p++) {
        if (!nsk_palette_validate(colors, &palettes->plane[p])) {
            return false;
        }
    }

    for (size_t g = 0; g < NSK_PALETTESIZE_GROUPS; g++) {
        const union nsk_type_color4 *cb =
            &palettes->plane[NSK_PLANE_BACKGROUND].group[g].color[0];
        const union nsk_type_color4 *cs =
            &palettes->plane[NSK_PLANE_SPRITES].group[g].color[0];

        if (cb->raw != cs->raw) {
            nsk_err(
                "Error: first colors of the %zu groups does not match: %s ≠ %s\n",
                g,
                nsk_string_color(cb->r, cb->g, cb->b),
                nsk_string_color(cs->r, cs->g, cs->b)
            );
            return false;
        }
    }

    return true;
}

/*!
 * \brief  Assigns the PPU colors to the palette using `index` field
 *
 * \param[in]       colors   The colors
 * \param[in,out]   palette  The palette
 * \return True if colors were assigned, false otherwise
 */
bool nsk_palette_setcolors(
    const struct nsk_type_ppucolors *colors,
    struct nsk_type_palette *palette
) {
    nsk_palette_validate_indexes(palette);

    for (size_t g = 0; g < NSK_PALETTESIZE_GROUPS; g++) {
        for (size_t c = 0; c < NSK_PALETTESIZE_COLORS; c++) {
            size_t index = palette->group[g].index[c];

            if (index >= NSK_PPUCOLORSTABLE_COUNT) {
                nsk_err(
                    "Error: invalid index for %s palette %zu:%zu: %zu\n",
                    nsk_conv_plane2string(palette->plane),
                    g,
                    c,
                    index
                );
                return false;
            }

            if (!colors->allowed[index]) {
                nsk_err(
                    "Error: color #%zu in the %s palette %zu:%zu is not allowed"
                    " in the PPU colors list\n",
                    index,
                    nsk_conv_plane2string(palette->plane),
                    g,
                    c
                );
                return false;
            }

            palette->group[g].color[c].raw = colors->colors[index].raw;
        }
    }

    palette->init.colors = true;
    return true;
}

/*!
 * \brief  Assigns the PPU colors to the palettes using `index` field
 *
 * \param[in]       colors    The colors
 * \param[in,out]   palettes  The palettes
 * \return True if colors were assigned, false otherwise
 */
bool nsk_palettes_setcolors(
    const struct nsk_type_ppucolors *colors,
    struct nsk_type_palettes *palettes
) {
    for (size_t p = 0; p < NSK_PLANES_COUNT; p++) {
        if (!nsk_palette_setcolors(colors, &palettes->plane[p])) {
            return false;
        }
    }

    return true;
}

/*!
 * \brief  Assigns palette indexes by colors
 *
 * \param[in]     colors    The colors
 * \param[in,out] palette   The palette
 */
bool nsk_palette_setindexes(
    const struct nsk_type_ppucolors *colors,
    struct nsk_type_palette *palette
) {
    if (!nsk_palette_validate_colors(palette)) {
        return false;
    }

    for (size_t g = 0; g < NSK_PALETTESIZE_GROUPS; g++) {
        for (size_t c = 0; c < NSK_PALETTESIZE_COLORS; c++) {
            size_t i;
            for (i = 0; i < NSK_PPUCOLORSTABLE_COUNT; i++) {
                if (colors->allowed[i] &&
                    palette->group[g].color[c].raw == colors->colors[i].raw
                ) {
                    palette->group[g].index[c] = i;
                    break;
                }
            }
            if (i == NSK_PPUCOLORSTABLE_COUNT) {
                nsk_err(
                    "Error: invalid color in the palette [%zu:%zu]: %s",
                    g,
                    c,
                    nsk_string_color(
                        palette->group[g].color[c].r,
                        palette->group[g].color[c].g,
                        palette->group[g].color[c].b
                    )
                );
                return false;
            }
        }
    }

    palette->init.indexes = true;
    return true;
}

/*!
 * \brief  Assigns palettes indexes by colors
 *
 * \param[in]     colors    The colors
 * \param[in,out] palettes  The palettes
 */
bool nsk_palettes_setindexes(
    const struct nsk_type_ppucolors *colors,
    struct nsk_type_palettes *palettes
) {
    for (size_t p = 0; p < NSK_PLANES_COUNT; p++) {
        if (!nsk_palette_setindexes(
            colors,
            &palettes->plane[p]
        )) {
            return false;
        }
    }

    return true;
}

/*!
 * \brief  Finds the color index in the selected palette
 *
 * \param[in] palette  The palette
 * \param[in] group The colors group
 * \return The color index, or (size_t)-1 on error
 */
size_t nsk_palette_getindex(
    const struct nsk_type_palette *palette,
    size_t group,
    const union nsk_type_color4 *color
) {
    if (!nsk_palette_validate_colors(palette)) {
        return (size_t)-1;
    }

    for (size_t i = 0; i < NSK_PALETTESIZE_COLORS; i++) {
        if (palette->group[group].color[i].raw == color->raw) {
            return i;
        }
    }

    nsk_err(
        "Error: color %s is not found in the %zu group of the %s palette\n",
        nsk_string_color(
            color->r,
            color->g,
            color->b
        ),
        group,
        nsk_conv_plane2string(palette->plane)
    );
    return (size_t)-1;
}
