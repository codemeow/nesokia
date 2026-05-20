#ifndef NSK_TYPE_PNGCOMPOSE
#define NSK_TYPE_PNGCOMPOSE

#include <stdbool.h>

#include "png/types/nsk_type_pngimage.h"
#include "types/nsk_type_ppucolors.h"
#include "types/nsk_type_palettes.h"
#include "types/nsk_type_pattable.h"

/*!
 * \brief  Creates full Nesokia PNG image from separate components
 *
 * \param[in]  colors    The PPU colors
 * \param[in]  palettes  The local palettes
 * \param[in]  patleft   The left pattern table
 * \param[in]  patright  The pattern tables
 * \return Composite (full) image, or NULL on error
 */
struct nsk_type_pngimage *nsk_pngimage_composesave(
    const struct nsk_type_ppucolors *colors,
    const struct nsk_type_palettes  *palettes,
    const struct nsk_type_pattables *pattables
);

/*!
 * \brief  Reads the composite Nesokia PNG image
 *
 * \param[in]  filename   The filename
 * \param[out] colors     The colors
 * \param[out] palettes   The palettes
 * \param[out] pattables  The pattern tables
 * \return True if the image was read, false otherwise
 *
 * \note Arguments cannot be NULL.
 */
bool nsk_pngimage_composeread(
    const char *filename,
    struct nsk_type_ppucolors *colors,
    struct nsk_type_palettes  *palettes,
    struct nsk_type_pattables *pattables
);

#endif
