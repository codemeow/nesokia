#ifndef NSK_PPUCOLORS_SAVEACO
#define NSK_PPUCOLORS_SAVEACO

#include <stdbool.h>

#include "base/nsk_util_attributes.h"
#include "types/nsk_type_ppucolors.h"

/*!
 * \brief  Saves the PPU colors as .aco (Photoshop Color Swatch) file
 *
 * \param[in] filename  The filename
 * \param[in] colors    The colors
 * \return True if the PPU colors were saved, false otherwise
 */
nsk_attr_result_unused
bool nsk_ppucolors_saveaco(
    const char *filename,
    const struct nsk_type_ppucolors *colors
);

#endif
