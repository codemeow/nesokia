#ifndef NSK_PPUCOLORS_SAVEGPL
#define NSK_PPUCOLORS_SAVEGPL

#include "../../types/nsk_type_ppucolors.h"

/*!
 * \brief  Saves the PPU colors as .gpl (GIMP palette) file
 *
 * \param[in] filename  The filename
 * \param[in] colors    The colors
 */
void nsk_ppucolors_savegpl(
    const char *filename,
    const struct nsk_type_ppucolors *colors
);

#endif