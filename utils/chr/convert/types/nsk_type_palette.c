#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <threads.h>

#include "../types/nsk_type_palette.h"

#include "../process/nsk_process_utils.h"
#include "../process/nsk_process_vars.h"
#include "../types/nsk_type_color.h"
#include "../types/nsk_type_colors.h"
#include "../types/nsk_type_planes.h"
#include "../utils/log/nsk_log_err.h"
#include "../utils/log/nsk_log_inf.h"
#include "../utils/nsk_util_size.h"

/*!
 * \brief  Number of static strings in functions, returning static strings
 */
#define _STATIC_CAROUSEL_SIZE (5)

/*!
 * List of the palettes
 */
static const struct {
    enum nsk_plane_list plane;  /*!< Plane of the palette   */
    size_t x;                   /*!< X data start position  */
    size_t y;                   /*!< Y data start position  */
} _table_palettes[] = {
    { NSK_PLANE_BACKGROUND, NSK_PALETTEPOS_BACK_X,   NSK_PALETTEPOS_BACK_Y   },
    { NSK_PLANE_SPRITES,    NSK_PALETTEPOS_SPRITE_X, NSK_PALETTEPOS_SPRITE_Y },
};

/*!
 * \brief  Converts the image pixel into the palette color
 *
 * \param[in] pixel  The pixel
 * \return  Converted pixel
 */
static struct nsk_type_color _palette_color(const union nsk_type_pixel *pixel) {
    struct nsk_type_color color = {
        .rgb.raw = pixel->raw
    };

    color.palette.color = nsk_colors_index(&color);
    return color;
}

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
 * \brief  Reads single palette
 *
 * \param[in] plane  The plane
 * \param[in] x      X data start position
 * \param[in] y      Y data start position
 */
static void _palette_read(
    enum nsk_plane_list plane,
    size_t x,
    size_t y
) {
    nsk_inf(
        "# %s palettes ($3f-)\n",
        nsk_conv_plane2string(plane)
    );

    nsk_inf("           ");
    for (size_t c = 0; c < NSK_PALETTE_COLORS; c++) {
        nsk_inf("   -%s ", _string_binary(c));
    }
    nsk_inf("\n");

    for (size_t g = 0; g < NSK_PALETTE_GROUPS; g++) {
        nsk_inf("  %%%04d %s- ", (int)plane, _string_binary(g));
        for (size_t c = 0; c < NSK_PALETTE_COLORS; c++) {
            size_t xshift =
                (g * NSK_PALETTE_COLORS + c) * NSK_PALETTECELL_WIDTH;
            size_t ypix = y + NSK_PALETTECELL_PADY;
            size_t xpix = x + NSK_PALETTECELL_PADX + xshift;

            struct nsk_type_color color = _palette_color(
                &nsk_input.image->data[ypix][xpix]
            );

            nsk_input.palettes.planes[plane].group[g].color[c] = color;
            nsk_inf(
                "%s ",
                nsk_string_color(color.rgb.r, color.rgb.g, color.rgb.b)
            );
        }
        nsk_inf("\n");
    }
    nsk_inf("\n");
}

/*!
 * \brief  Reads the local palettes from the input file
 */
void nsk_palettes_read(void) {
    for (size_t i = 0; i < NSK_SIZE(_table_palettes); i++) {
        _palette_read(
            _table_palettes[i].plane,
            _table_palettes[i].x,
            _table_palettes[i].y
        );
    }
}

/*!
 * \brief  Validates the single local palette
 *
 * \param[in] plane  The plane
 */
static void _palette_validate(enum nsk_plane_list plane) {
    for (size_t g = 0; g < NSK_PALETTE_GROUPS; g++) {
        for (size_t c = 0;c < NSK_PALETTE_COLORS; c++) {
            struct nsk_type_color *color =
                &nsk_input.palettes.planes[plane].group[g].color[c];

            if (color->palette.color == NSK_COLOR_INVALID) {
                nsk_err(
                    "Color %s (%%%s%s) of the %s palette doesn't "
                        "match any global palette's color\n",
                    nsk_string_color(color->rgb.r, color->rgb.g, color->rgb.b),
                    _string_binary(g),
                    _string_binary(c),
                    nsk_conv_plane2string(plane)
                );
                exit(EXIT_FAILURE);
            }
        }
    }
}

/*!
 * \brief  Validates all palettes at once for the same 0th color
 */
static void _palettes_validate(void) {
    for (size_t g = 0; g < NSK_PALETTE_COLORS; g++) {
        struct nsk_type_color *color_b =
            &nsk_input.palettes.planes[NSK_PLANE_BACKGROUND].group[g].color[0];
        struct nsk_type_color *color_s =
            &nsk_input.palettes.planes[NSK_PLANE_SPRITES   ].group[g].color[0];

        if (color_b->rgb.raw != color_s->rgb.raw) {
            nsk_err(
                "Colors -00 at group %s- must be identical "
                    "for background and sprites (%s ≠ %s)\n",
                _string_binary(g),
                nsk_string_color(color_b->rgb.r, color_b->rgb.g, color_b->rgb.b),
                nsk_string_color(color_s->rgb.r, color_s->rgb.g, color_s->rgb.b)
            );
            exit(EXIT_FAILURE);
        }
    }
}

/*!
 * \brief  Validates the local palettes
 */
void nsk_palettes_validate(void) {
    for (size_t i = 0; i < NSK_SIZE(_table_palettes); i++) {
        _palette_validate(
            _table_palettes[i].plane
        );
    }

    _palettes_validate();
}
