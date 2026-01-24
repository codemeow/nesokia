#if defined(NSK_MODULE_COMPOSE) && defined(NSK_MODULE_PNG)

#include "../../png/types/nsk_type_pngtile.h"

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
) {
    for (size_t h = 0; h < NSK_TILESIZE_HEIGHT; h++) {
        for (size_t w = 0; w < NSK_TILESIZE_WIDTH; w++) {
            tile->pixel[h][w].color = image->data[starty + h][startx + w];
        }
    }
}

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
) {
    nsk_tile_validate_colors(tile);

    for (size_t h = 0; h < NSK_TILESIZE_HEIGHT; h++) {
        for (size_t w = 0; w < NSK_TILESIZE_WIDTH; w++) {
            image->data[starty + h][startx + w] =
                tile->pixel[h][w].color;
        }
    }
}

#endif