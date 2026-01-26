#ifndef NSK_PPUCOLORS_SAVEASE
#define NSK_PPUCOLORS_SAVEASE

#include "../../types/nsk_type_ppucolors.h"

/*!
 * \brief  Saves the PPU colors as .ase (Adobe Swatch Exchange) file
 *
 * \param[in] filename  The filename
 * \param[in] colors    The colors
 */
void nsk_ppucolors_savease(
    const char *filename,
    const struct nsk_type_ppucolors *colors
);

#endif