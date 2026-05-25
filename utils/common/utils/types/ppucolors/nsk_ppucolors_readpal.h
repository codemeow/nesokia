#ifndef NSK_PPUCOLORS_READPAL
#define NSK_PPUCOLORS_READPAL

#include "base/nsk_util_attributes.h"
#include "types/nsk_type_ppucolors.h"

/*!
 * \brief  Reads the PPU colors from the .pal file
 *
 * \param[in]  filename  The filename
 * \param[out] colors    The PPU colors
 * \return True if the PPU colors were read, false otherwise
 */
nsk_attr_result_unused
bool nsk_ppucolors_readpal(
    const char *filename,
    struct nsk_type_ppucolors *colors
);

#endif
