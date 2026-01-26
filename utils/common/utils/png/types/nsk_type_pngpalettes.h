#ifndef NSK_TYPE_PNGPALETTES
#define NSK_TYPE_PNGPALETTES

#include "../../types/nsk_type_palettes.h"
#include "../../png/types/nsk_type_pngimage.h"

/*!
 * \brief  Reads local palettes from Nesokia PNG component
 *
 * \param[in] filename  The filename
 * \return Local palettes
 */
struct nsk_type_palettes nsk_palettes_readpng(
    const char *filename
);

/*!
 * \brief  Converts local palettes into composite component
 *
 * \param[in] palettes  The palettes
 * \return Nesokia PNG component image
 */
struct nsk_type_pngimage *nsk_palettes_convtopng(
    const struct nsk_type_palettes *palettes
);

/*!
 * \brief  Converts PNG composite component to local paletets
 *
 * \param[in]  image     The image
 * \param[in]  x         Component X start position
 * \param[in]  y         Component Y start position
 * \param[out] palettes  The palettes
 */
void nsk_palettes_convfrompng(
    const struct nsk_type_pngimage *image,
    size_t x,
    size_t y,
    struct nsk_type_palettes *palettes
);

#endif