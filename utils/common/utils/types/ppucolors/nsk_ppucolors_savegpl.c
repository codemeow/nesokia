#include <stdlib.h>
#include <string.h>

#include "../../types/ppucolors/nsk_ppucolors_savegpl.h"
#include "../../nsk_util_cleanup.h"
#include "../../nsk_util_size.h"
#include "../../types/ppucolors/nsk_ppucolors_common.h"

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

    nsk_auto_fclose FILE *file = nsk_ppucolors_fopen(filename, "wb");

    snprintf(
        buffer,
        sizeof(buffer),
        "%s\n",
        gpl_magic
    );
    nsk_ppucolors_fwrite(buffer, strlen(buffer), file, filename);

    snprintf(
        buffer,
        sizeof(buffer),
        "%s%s\n",
        gpl_name,
        basename(filename)
    );
    nsk_ppucolors_fwrite(buffer, strlen(buffer), file, filename);

    snprintf(
        buffer,
        sizeof(buffer),
        "%s%d\n",
        gpl_columns,
        (int)NSK_PPUCOLORSTABLE_WIDTH
    );
    nsk_ppucolors_fwrite(buffer, strlen(buffer), file, filename);

    for (size_t i = 0; i < NSK_SIZE(colors->colors); i++) {
        if (!colors->allowed[i]) {
            continue;
        }

        snprintf(
            buffer,
            sizeof(buffer),
            "%3hhu %3hhu %3hhu\n",
            colors->colors[i].r,
            colors->colors[i].g,
            colors->colors[i].b
        );
        nsk_ppucolors_fwrite(buffer, strlen(buffer), file, filename);
    }
}
