#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <nsk_util_meta.h>

#include "../process/nsk_process_utils.h"
#include "../process/nsk_process_vars.h"
#include "../types/nsk_type_color.h"
#include "../types/nsk_type_colors.h"

/*!
 * \brief  Checks if this color is not marked as invalid
 *
 * \note The valid (true) color has the whole cell single-colored
 *       The invalid (false) color has any markings of other color in the cell
 *
 * \param[in] color   The color
 * \param[in] startx  X position of the cell in the image
 * \param[in] starty  Y position of the cell in the image
 * \return True if this cell is allowed to use
 */
static bool _color_isallowed(
    const struct nsk_type_color *color,
    size_t startx,
    size_t starty
) {
    for (size_t h = 0; h < NSK_COLORSCELL_HEIGHT; h++) {
        for (size_t w = 0; w < NSK_COLORSCELL_WIDTH; w++) {
            const size_t posy = starty + h;
            const size_t posx = startx + w;

            if (nsk_input.image->data[posy][posx].raw != color->rgb.raw) {
                return false;
            }
        }
    }
    return true;
}

/*!
 * \brief  Reads the global palette from the input file
 */
void nsk_colors_read(void) {
    nsk_inf("# Global palette:\n");

    nsk_inf("     ");
    for (size_t x = 0; x < NSK_COLORS_WIDTH; x++) {
        nsk_inf("   -%1zx  ", x);
    }
    nsk_inf("\n");

    for (size_t y = 0; y < NSK_COLORS_HEIGHT; y++) {
        nsk_inf("  $%1zx- ", y);
        for (size_t x = 0; x < NSK_COLORS_WIDTH; x++) {
            const size_t index = y * NSK_COLORS_WIDTH + x;
            const size_t posy  = NSK_COLORSPOS_Y + y * NSK_COLORSCELL_HEIGHT;
            const size_t posx  = NSK_COLORSPOS_X + x * NSK_COLORSCELL_WIDTH;

            struct nsk_type_color color = {
                .rgb.raw = nsk_input.image->data[posy][posx].raw
            };

            nsk_input.colors.colors [index] = color;
            nsk_input.colors.allowed[index] = _color_isallowed(
                &color,
                posx,
                posy
            );

            if (nsk_input.colors.allowed[index]) {
                nsk_inf(
                    "%s ",
                    nsk_string_color(color.rgb.r, color.rgb.g, color.rgb.b)
                );
            } else {
                nsk_inf(
                    "%s ",
                    "------"
                );
            }
        }
        nsk_inf("\n");
    }

    nsk_inf("\n");
}

/*!
 * \brief  Validate for the number of allowed colors
 */
void _validate_allowed(void) {
    for (size_t i = 0; i < NSK_COLORS_COUNT; i++) {
        if (nsk_input.colors.allowed[i]) {
            return;
        }
    }

    nsk_err(
        "Invalid global palette: at least one allowed color is required"
    );
    exit(EXIT_FAILURE);
}

/*!
 * \brief  Validates the global palette
 */
void nsk_colors_validate(void) {
    /*
     * The only validation about global palette is the number of valid colors -
     * it obviously should be more than 1
     */
    _validate_allowed();
}

/*!
 * \brief  Lookups the index of the color in the global palette
 *
 * \param[in] color  The color
 * \return Index or NSK_COLOR_INVALID
 */
unsigned nsk_colors_index(const struct nsk_type_color *color) {
    for (size_t i = 0; i < NSK_COLORS_COUNT; i++) {
        if (nsk_input.colors.allowed[i] &&
            nsk_input.colors.colors [i].rgb.raw == color->rgb.raw) {
            return i;
        }
    }

    return NSK_COLOR_INVALID;
}
