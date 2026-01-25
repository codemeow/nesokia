#ifndef NSK_PPUCOLORS_SAVEACO
#define NSK_PPUCOLORS_SAVEACO

#include "../../types/nsk_type_ppucolors.h"

/*!
 * \brief  Saves the PPU colors as .aco (Photoshop Color Swatch) file
 *
 * \param[in] filename  The filename
 * \param[in] colors    The colors
 */
void nsk_ppucolors_saveaco(
    const char *filename,
    const struct nsk_type_ppucolors *colors
);

#endif