#include <assert.h>
#include <stdlib.h>

#include "../types/nsk_type_nametable.h"
#include "../arguments/nsk_args_options.h"
#include "../process/nsk_process_vars.h"
#include "../types/nsk_type_palette.h"
#include "../types/nsk_type_planes.h"
#include "../utils/log/nsk_log_err.h"
#include "../utils/log/nsk_log_inf.h"
#include "../utils/nsk_util_assert.h"
#include "../utils/nsk_util_size.h"
#include "../process/nsk_process_utils.h"

/*!
 * \brief Nametables positioning
 */
const struct {
    size_t x; /*!< X position data start */
    size_t y; /*!< Y position data start */
} _table_namespaces[] = {
    [NSK_NAMETABLEADDR_0000] = {
        .x = NSK_NAMETABLEPOS_LEFT_X,
        .y = NSK_NAMETABLEPOS_LEFT_Y
    },
    [NSK_NAMETABLEADDR_1000] = {
        .x = NSK_NAMETABLEPOS_RIGHT_X,
        .y = NSK_NAMETABLEPOS_RIGHT_Y
    }
};

NSK_STATIC_ASSERT(
    NSK_SIZE(_table_namespaces) == NSK_NAMETABLEADDRS_COUNT,
    "_table_namespaces size should match the number of nametable addresses"
);

/*!
 * \brief  Converts the enum value to the raw value
 *
 * \param[in] address  The address
 * \return Raw address value
 */
unsigned nsk_conv_address2value(enum nsk_nametable_address address) {
    switch (address) {
        case NSK_NAMETABLEADDR_0000: return 0x0000;
        case NSK_NAMETABLEADDR_1000: return 0x1000;

        default:
            nsk_err("Unknown address value: %d\n", (int)address);
            abort();
    }
}

/*!
 * \brief  Converts the nametable left/right address into the plane
 *
 * \param[in] address  The address
 * \return Background or sprites plane
 */
enum nsk_plane_list _nametable_plane(enum nsk_nametable_address address) {
    static const enum nsk_plane_list _table
        [NSK_PPUCTRL34S_COUNT][NSK_PLANES_COUNT] = {
        [NSK_PPUCTRL34_NORMAL] = {
            [NSK_NAMETABLEADDR_0000] = NSK_PLANE_BACKGROUND,
            [NSK_NAMETABLEADDR_1000] = NSK_PLANE_SPRITES,
        },
        [NSK_PPUCTRL34_INVERT] = {
            [NSK_NAMETABLEADDR_0000] = NSK_PLANE_SPRITES,
            [NSK_NAMETABLEADDR_1000] = NSK_PLANE_BACKGROUND,
        },
    };

    return _table[nsk_options_program.ppuctrl34][address];
}

/*!
 * \brief  Checks if the provided color is non 0th color for this plane
 *
 * \note The color that doesn't match any palette color is "non 0th too"
 *
 * \param[in] plane  The plane
 * \param[in] color  The color
 * \return True if the color is non 0th
 */
static bool _palette_isnon0(
    enum nsk_plane_list plane,
    const struct nsk_type_color *color
) {
    for (size_t g = 0; g < NSK_PALETTE_GROUPS; g++) {
        for (size_t c = 1; c < NSK_PALETTE_COLORS; c++) {
            struct nsk_type_color *pcolor =
                &nsk_input.palettes.planes[plane].group[g].color[c];

            if (pcolor->rgb.raw == color->rgb.raw) {
                return true;
            }
        }
    }
    return false;
}

/*!
 * \brief  Reads on single tile
 *
 * \param[in,out]   tile   The tile
 * \param[in]       plane  The plane
 * \param[in]       x      Tile X data start position
 * \param[in]       y      Tile Y data start position
 * \return True if tile contains at least one non-0th palette color
 * (thus non-empty)
 */
static bool _read_tile(
    struct nsk_type_tile *tile,
    enum nsk_plane_list plane,
    size_t x,
    size_t y
) {
    bool nonempty = false;
    struct nsk_type_color color0 = {
        .rgb.raw = nsk_input.image->data[y][x].raw
    };

    if (_palette_isnon0(plane, &color0)) {
        nonempty = true;
    }

    for (size_t h = 0; h < NSK_NAMETABLECELL_HEIGHT; h++) {
        for (size_t w = 0; w < NSK_NAMETABLECELL_WIDTH; w++) {
            struct nsk_type_color color = {
                .rgb.raw = nsk_input.image->data[y + h][x + w].raw
            };

            tile->pixel[h][w] = color;

            if (color.rgb.raw != color0.rgb.raw) {
                nonempty = true;
            }
        }
    }

    return nonempty;
}

/*!
 * \brief  Reads single nametable from the image
 *
 * \param[in] plane    The plane
 * \param[in] address  The address
 * \param[in] x        X position data start
 * \param[in] y        Y position data start
 */
void _nametable_read(
    enum nsk_plane_list plane,
    enum nsk_nametable_address address,
    size_t x,
    size_t y
) {
    struct nsk_type_nametable *nametable =
        &nsk_input.nametables.plane[plane];

    nametable->address = address;
    nsk_inf(
        "# %s nametable ($%04x)\n",
        nsk_conv_plane2string(plane),
        nsk_conv_address2value(address)
    );

    nsk_inf("        ");
    for (size_t w = 0; w < NSK_NAMETABLE_WIDTH; w++) {
        nsk_inf("-%zx ", w);
    }
    nsk_inf("\n");

    for (size_t ty = 0; ty < NSK_NAMETABLE_HEIGHT; ty++) {
        nsk_inf("  $%03zx- ", nsk_conv_address2value(address) / 0x10 + ty);

        for (size_t tx = 0; tx < NSK_NAMETABLE_WIDTH; tx++) {
            bool empty = !_read_tile(
                &nametable->tile[ty][tx],
                plane,
                x + tx * NSK_NAMETABLECELL_WIDTH,
                y + ty * NSK_NAMETABLECELL_HEIGHT
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
 * \brief  Reads the nametables from the input image
 */
void nsk_nametables_read(void) {
    for (size_t i = 0; i < NSK_SIZE(_table_namespaces); i++) {
        size_t index = _nametable_plane(i);

        _nametable_read(
            index,
            i,
            _table_namespaces[index].x,
            _table_namespaces[index].y
        );
    }
}

/*!
 * \brief  Lists the provided colors with nsk_err
 *
 * \param[in] colors  The colors
 * \param[in] count   The colors count
 */
static void _colors_errlist(
    const struct nsk_type_color *colors,
    size_t count
) {
    for (size_t i = 0; i < count; i++) {
        nsk_err(
            "%s%s",
            i == 0 ? ", " : "",
            nsk_string_color(
                colors[i].rgb.r,
                colors[i].rgb.g,
                colors[i].rgb.b
            )
        );
    }
}

/*!
 * \brief  Validates single tile
 *
 * \param[in,out]   tile     The tile
 * \param[in]       plane    The plane
 * \param[in]       address  The nametableaddress
 * \param[in]       x        Tile X position
 * \param[in]       y        Tile Y position
 */
static void _validate_tile(
    struct nsk_type_tile *tile,
    enum nsk_plane_list plane,
    enum nsk_nametable_address address,
    size_t x,
    size_t y
) {
    struct nsk_type_color colors[NSK_PALETTE_COLORS];
    size_t used = 0;

    for (size_t h = 0; h < NSK_NAMETABLECELL_HEIGHT; h++) {
        for (size_t w = 0; w < NSK_NAMETABLECELL_WIDTH; w++) {
            struct nsk_type_color color = tile->pixel[h][w];

            size_t i;
            for (i = 0; i < used; i++) {
                if (colors[i].rgb.raw == color.rgb.raw) {
                    break;
                }
            }
            if (i == used) {
                if (used == NSK_PALETTE_COLORS) {
                    nsk_err(
                        "Tile $%03zx%zx of the %s nametable ($%04x) contains "
                            "more than %d colors: ",
                        nsk_conv_address2value(address) / 0x10 + y,
                        x,
                        nsk_conv_plane2string(plane),
                        nsk_conv_address2value(address),
                        (int)NSK_PALETTE_COLORS
                    );
                    _colors_errlist(colors, used);
                    nsk_err(
                        " and %s\n",
                        nsk_string_color(
                            color.rgb.r,
                            color.rgb.g,
                            color.rgb.b
                        )
                    );
                    exit(EXIT_FAILURE);

                } else {
                    colors[used++].rgb.raw = color.rgb.raw;
                }
            }
        }
    }

    struct nsk_type_palette *palette = &nsk_input.palettes.planes[plane];
    size_t g;
    for (g = 0; g < NSK_PALETTE_GROUPS; g++) {
        bool match = true;

        for (size_t i = 0; i < used; i++) {
            size_t c;
            for (c = 0; c < NSK_PALETTE_COLORS; c++) {
                if (palette->group[g].color[c].rgb.raw == colors[i].rgb.raw) {
                    break;
                }
            }

            if (c == NSK_PALETTE_COLORS) {
                match = false;
                break;
            }
        }

        if (match) {
            tile->palette = g;
            break;
        }
    }

    if (g == NSK_PALETTE_GROUPS) {
        nsk_err(
            "Tile $%03zx%zx of the %s nametable ($%04x) does not match any "
                "palette. Tile colors: ",
            nsk_conv_address2value(address) / 0x10 + y,
            x,
            nsk_conv_plane2string(plane),
            nsk_conv_address2value(address)
        );
        _colors_errlist(colors, used);
        nsk_err("\n");
        exit(EXIT_FAILURE);
    }
}

/*!
 * \brief  Validates single nametable
 *
 * \param[in] plane    The plane
 * \param[in] address  The address
 */
static void _nametable_validate(
    enum nsk_plane_list plane,
    enum nsk_nametable_address address
) {
    struct nsk_type_nametable *nametable =
        &nsk_input.nametables.plane[plane];

    for (size_t ty = 0; ty < NSK_NAMETABLE_HEIGHT; ty++) {
        for (size_t tx = 0; tx < NSK_NAMETABLE_WIDTH; tx++) {
            _validate_tile(
                &nametable->tile[ty][tx],
                plane,
                address,
                tx,
                ty
            );
        }
    }
}

/*!
 * \brief  Validates the nametables
 */
void nsk_nametables_validate(void) {
    for (size_t i = 0; i < NSK_SIZE(_table_namespaces); i++) {
        size_t index = _nametable_plane(i);

        _nametable_validate(
            index,
            i
        );
    }
}