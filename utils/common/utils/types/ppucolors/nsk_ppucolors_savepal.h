#ifndef NSK_PPUCOLORS_SAVEPAL
#define NSK_PPUCOLORS_SAVEPAL

#include "../../types/nsk_type_ppucolors.h"

/*!
 * \brief  Saves the PPU colors as .pal file
 *
 * \param[in] filename  The filename
 * \param[in] colors    The colors
 */
void nsk_ppucolors_savepal(
    const char *filename,
    const struct nsk_type_ppucolors *colors
);

#endif