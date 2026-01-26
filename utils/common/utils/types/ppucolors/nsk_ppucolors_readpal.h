#ifndef NSK_PPUCOLORS_READPAL
#define NSK_PPUCOLORS_READPAL

#include "../../types/nsk_type_ppucolors.h"

/*!
 * \brief  Reads the PPU colors from the .pal file
 *
 * \param[in] filename  The filename
 * \return PPU colors
 */
struct nsk_type_ppucolors nsk_ppucolors_readpal(
    const char *filename
);

#endif
