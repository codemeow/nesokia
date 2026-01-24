#ifndef NSK_TYPE_PNGTILE
#define NSK_TYPE_PNGTILE

#if defined(NSK_MODULE_COMPOSE) && defined(NSK_MODULE_PNG)

#include "../../types/nsk_type_tile.h"
#include "../../png/types/nsk_type_pngimage.h"

/*!
 * \brief  Reads the selected tile from Nesokia PNG from selected position
 *
 * \param[in] image   The image
 * \param[in] startx  The startx
 * \param[in] starty  The starty
 * \param     tile    The tile
 */
void nsk_tile_readpng(
    const struct nsk_type_pngimage *image,
    size_t startx,
    size_t starty,
    struct nsk_type_tile *tile
);

/*!
 * \brief  Converts selected tile to PNG data
 *
 * \param[in]     tile    The tile
 * \param[in]     startx  The startx
 * \param[in]     starty  The starty
 * \param[in,out] image   The image
 */
void nsk_tile_convtopng(
    const struct nsk_type_tile *tile,
    size_t startx,
    size_t starty,
    struct nsk_type_pngimage *image
);

#endif

#endif