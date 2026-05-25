#include "types/ppucolors/nsk_ppucolors_readpal.h"
#include "types/ppucolors/nsk_ppucolors_common.h"
#include "base/nsk_util_size.h"
#include "base/nsk_util_cleanup.h"

/*!
 * \brief  Reads the PPU colors from the .pal file
 *
 * \param[in]  filename  The filename
 * \param[out] colors    The PPU colors
 * \return True if the PPU colors were read, false otherwise
 */
bool nsk_ppucolors_readpal(
    const char *filename,
    struct nsk_type_ppucolors *colors
) {
    *colors = (struct nsk_type_ppucolors){ 0 };
    nsk_auto_fclose FILE *file = nsk_ppucolors_fopen(filename, "rb");
    if (!file) {
        return false;
    }

    for (size_t i = 0; i < NSK_SIZE(colors->colors); i++) {
        if (!nsk_ppucolors_fread(
            &colors->colors[i].r,
            sizeof(colors->colors[i].r),
            file,
            filename
        ) || !nsk_ppucolors_fread(
            &colors->colors[i].g,
            sizeof(colors->colors[i].g),
            file,
            filename
        ) || !nsk_ppucolors_fread(
            &colors->colors[i].b,
            sizeof(colors->colors[i].b),
            file,
            filename
        )) {
            return false;
        }
        colors->allowed[i] = true;
    }

    return true;
}
