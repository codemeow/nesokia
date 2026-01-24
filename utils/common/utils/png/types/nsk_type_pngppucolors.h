#ifndef NSK_TYPE_PNGPPUCOLORS
#define NSK_TYPE_PNGPPUCOLORS

#if defined(NSK_MODULE_COMPOSE) && defined(NSK_MODULE_PNG)

#include "../../types/nsk_type_ppucolors.h"
#include "../../png/types/nsk_type_pngimage.h"
/*!
 * \brief  Reads the PPU colors from Nesokia PNG component
 *
 * \param[in] filename  The filename
 * \return PPU colors
 */
struct nsk_type_ppucolors nsk_ppucolors_readpng(
    const char *filename
);

/*!
 * \brief  Converts PPU colors into composite component
 *
 * \param[in] colors  The colors
 * \return Nesokia PNG component image
 */
struct nsk_type_pngimage *nsk_ppucolors_convtopng(
    const struct nsk_type_ppucolors *colors
);

/*!
 * \brief  Reads the PPU colors from Nesokia PNG template component
 *
 * \param[in] filename  The filename
 * \return PPU colors
 */
struct nsk_type_ppucolors nsk_ppucolors_convfrompng(
    const struct nsk_type_pngimage *image,
    size_t x,
    size_t y,
    struct nsk_type_ppucolors *colors
);

#endif

#endif