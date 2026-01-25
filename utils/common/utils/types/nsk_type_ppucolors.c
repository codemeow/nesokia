#include <stdlib.h>

#include "../types/nsk_type_ppucolors.h"
#include "../log/nsk_log_err.h"
#include "../log/nsk_log_inf.h"
#include "../nsk_util_size.h"
#include "../strings/nsk_strings_ansi.h"

/*!
 * \brief  Shows the PPU colors as ANSI colored output
 *
 * \param[in] colors  The colors
 */
void nsk_ppucolors_show(
    const struct nsk_type_ppucolors *colors
) {
    nsk_inf("# PPU colors:\n");

    nsk_inf("     ");
    for (size_t x = 0; x < NSK_PPUCOLORSTABLE_WIDTH; x++) {
        nsk_inf("   -%1zx  ", x);
    }
    nsk_inf("\n");

    for (size_t y = 0; y < NSK_PPUCOLORSTABLE_HEIGHT; y++) {
        nsk_inf("  $%1zx- ", y);
        for (size_t x = 0; x < NSK_PPUCOLORSTABLE_WIDTH; x++) {
            const size_t index = y * NSK_PPUCOLORSTABLE_WIDTH + x;

            if (colors->allowed[index]) {
                nsk_inf(
                    "%s ",
                    nsk_string_color(
                        colors->colors[index].r,
                        colors->colors[index].g,
                        colors->colors[index].b
                    )
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
 * \brief  Lookups the index of the color in the PPU colors list
 *
 * \param[in] colos PPU colors list
 * \param[in] color  The color
 * \return Index or NSK_PPUCOLOR_INVALID
 */
size_t nsk_ppucolors_lookup(
    const struct nsk_type_ppucolors *colors,
    const union nsk_type_color4 *color
) {
        for (size_t i = 0; i < NSK_SIZE(colors->colors); i++) {
        if (colors->allowed[i] &&
            colors->colors [i].raw == color->raw) {
            return i;
        }
    }

    return NSK_PPUCOLOR_INVALID;
}

/*!
 * \brief  Validates the PPU colors list
 *
 * \param[in] colors  The colors
 */
void nsk_ppucolors_validate(
    const struct nsk_type_ppucolors *colors
) {
    for (size_t i = 0; i < NSK_PPUCOLORSTABLE_COUNT; i++) {
        if (colors->allowed[i]) {
            return;
        }
    }

    nsk_err(
        "Invalid PPU palette: at least one allowed color is required"
    );
    exit(EXIT_FAILURE);
}