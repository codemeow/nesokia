#if defined(NSK_MODULE_COMPOSE)

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../types/nsk_type_ppucolors.h"
#include "../error/nsk_util_errno.h"
#include "../io/nsk_io_fopen.h"
#include "../log/nsk_log_err.h"
#include "../log/nsk_log_inf.h"
#include "../nsk_util_cleanup.h"
#include "../nsk_util_size.h"
#include "../strings/nsk_strings_ansi.h"

/*!
 * \brief  Reads the PPU colors from the .pal file
 *
 * \param[in] filename  The filename
 * \return PPU colors
 */
struct nsk_type_ppucolors nsk_ppucolors_readpal(
    const char *filename
) {
    struct nsk_type_ppucolors colors;
    nsk_auto_fclose FILE *file = nsk_io_fopen(
        filename,
        "rb"
    );
    if (!file) {
        nsk_err(
            "Error: cannot open file \"%s\" for PPU colors reading.\n"
            "Possible reason: %s\n",
            filename,
            nsk_util_strerror(errno)
        );
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < NSK_SIZE(colors.colors); i++) {
        if (fread(&colors.colors[i].r, sizeof(colors.colors[i].r), 1, file) != 1 ||
            fread(&colors.colors[i].g, sizeof(colors.colors[i].g), 1, file) != 1 ||
            fread(&colors.colors[i].b, sizeof(colors.colors[i].b), 1, file) != 1
        ) {
            nsk_err(
                "Error: cannot read file \"%s\" for PPU colors reading.\n"
                "Possible reason: %s\n",
                filename,
                nsk_util_strerror(errno)
            );
            exit(EXIT_FAILURE);
        }
    }

    return colors;
}

/*!
 * \brief  Saves the PPU colors as .pal (NES emulator palette) file
 *
 * \param[in] filename  The filename
 * \param[in] colors    The colors
 */
void nsk_ppucolors_savepal(
    const char *filename,
    const struct nsk_type_ppucolors *colors
) {
    nsk_auto_fclose FILE *file = nsk_io_fopen(
        filename,
        "wb"
    );
    if (!file) {
        nsk_err(
            "Error: cannot open file \"%s\" for PPU colors writing.\n"
            "Possible reason: %s\n",
            filename,
            nsk_util_strerror(errno)
        );
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < NSK_SIZE(colors->colors); i++) {
        if (fwrite(&colors->colors[i].r, sizeof(colors->colors[i].r), 1, file) != 1 ||
            fwrite(&colors->colors[i].g, sizeof(colors->colors[i].g), 1, file) != 1 ||
            fwrite(&colors->colors[i].b, sizeof(colors->colors[i].b), 1, file) != 1
        ) {
            nsk_err(
                "Error: cannot write file \"%s\" for PPU colors writing.\n"
                "Possible reason: %s\n",
                filename,
                nsk_util_strerror(errno)
            );
            exit(EXIT_FAILURE);
        }
    }
}

/*!
 * \brief  Saves the PPU colors as .gpl (GIMP palette) file
 *
 * \param[in] filename  The filename
 * \param[in] colors    The colors
 */
void nsk_ppucolors_savegpl(
    const char *filename,
    const struct nsk_type_ppucolors *colors
) {
    static const char gpl_magic[] = "GIMP Palette";
    static const char gpl_name[]  = "Name: ";
    static const char gpl_columns[] = "Columns: ";
    char buffer[1024];

    nsk_auto_fclose FILE *file = nsk_io_fopen(
        filename,
        "wb"
    );
    if (!file) {
        nsk_err(
            "Error: cannot open file \"%s\" for PPU colors writing.\n"
            "Possible reason: %s\n",
            filename,
            nsk_util_strerror(errno)
        );
        exit(EXIT_FAILURE);
    }

    snprintf(buffer, sizeof(buffer), "%s\n", gpl_magic);
    if (fwrite(buffer, strlen(buffer), 1, file) != 1) {
        nsk_err(
            "Error: cannot write file \"%s\" for PPU colors writing.\n"
            "Possible reason: %s\n",
            filename,
            nsk_util_strerror(errno)
        );
        exit(EXIT_FAILURE);
    }

    snprintf(buffer, sizeof(buffer), "%s%s\n", gpl_name, basename(filename));
    if (fwrite(buffer, strlen(buffer), 1, file) != 1) {
        nsk_err(
            "Error: cannot write file \"%s\" for PPU colors writing.\n"
            "Possible reason: %s\n",
            filename,
            nsk_util_strerror(errno)
        );
        exit(EXIT_FAILURE);
    }

    snprintf(buffer, sizeof(buffer), "%s%d\n", gpl_columns, (int)NSK_PPUCOLORSTABLE_WIDTH);
    if (fwrite(buffer, strlen(buffer), 1, file) != 1) {
        nsk_err(
            "Error: cannot write file \"%s\" for PPU colors writing.\n"
            "Possible reason: %s\n",
            filename,
            nsk_util_strerror(errno)
        );
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < NSK_SIZE(colors->colors); i++) {
        snprintf(
            buffer,
            sizeof(buffer),
            "%3hhu %3hhu %3hhu\n",
            colors->colors[i].r,
            colors->colors[i].g,
            colors->colors[i].b
        );
    }
}

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

#endif