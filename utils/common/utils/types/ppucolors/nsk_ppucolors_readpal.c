#include "../../types/ppucolors/nsk_ppucolors_readpal.h"
#include "../../types/ppucolors/nsk_ppucolors_common.h"
#include "../../nsk_util_size.h"
#include "../../nsk_util_cleanup.h"

/*!
 * \brief  Reads the PPU colors from the .pal file
 *
 * \param[in] filename  The filename
 * \return PPU colors
 */
struct nsk_type_ppucolors nsk_ppucolors_readpal(
    const char *filename
) {
    struct nsk_type_ppucolors colors = { 0 };
    nsk_auto_fclose FILE *file = nsk_ppucolors_fopen(filename, "rb");

    for (size_t i = 0; i < NSK_SIZE(colors.colors); i++) {
        nsk_ppucolors_fread(
            &colors.colors[i].r,
            sizeof(colors.colors[i].r),
            file,
            filename
        );
        nsk_ppucolors_fread(
            &colors.colors[i].g,
            sizeof(colors.colors[i].g),
            file,
            filename
        );
        nsk_ppucolors_fread(
            &colors.colors[i].b,
            sizeof(colors.colors[i].b),
            file,
            filename
        );
        colors.allowed[i] = true;
    }

    return colors;
}
