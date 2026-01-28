#include <stdlib.h>

#include "../../types/ppucolors/nsk_ppucolors_savepal.h"
#include "../../nsk_util_cleanup.h"
#include "../../nsk_util_size.h"
#include "../../types/ppucolors/nsk_ppucolors_common.h"

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
    nsk_auto_fclose FILE *file = nsk_ppucolors_fopen(filename, "wb");

    for (size_t i = 0; i < NSK_SIZE(colors->colors); i++) {
        nsk_ppucolors_fwrite(
            &colors->colors[i].r,
            sizeof(colors->colors[i].r),
            file,
            filename
        );
        nsk_ppucolors_fwrite(
            &colors->colors[i].g,
            sizeof(colors->colors[i].g),
            file,
            filename
        );
        nsk_ppucolors_fwrite(
            &colors->colors[i].b,
            sizeof(colors->colors[i].b),
            file,
            filename
        );
    }
}