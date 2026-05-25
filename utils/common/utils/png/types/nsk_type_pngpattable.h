#ifndef NSK_TYPE_PNGPATTABLE
#define NSK_TYPE_PNGPATTABLE

#include <stdbool.h>

#include "base/nsk_util_attributes.h"
#include "types/nsk_type_pattable.h"
#include "png/types/nsk_type_pngimage.h"

/*!
 * \brief  Reads pattern table from Nesokia PNG component
 *
 * \param[in]  filename  The filename
 * \param[out] pattable  The pattern table
 * \return True if the pattern table was read, false otherwise
 */
nsk_attr_result_unused
bool nsk_pattable_readpng(
    const char *filename,
    struct nsk_type_pattable *pattable
);

/*!
 * \brief  Converts pattern table into composite component
 *
 * \param[in] pattable  The pattern table
 * \return Nesokia PNG component image, or NULL on error
 */
nsk_attr_result_unused
struct nsk_type_pngimage *nsk_pattable_convtopng(
    const struct nsk_type_pattable *pattable
);

/*!
 * \brief  Converts PNG composite component to pattern table
 *
 * \param[in]  image     The image
 * \param[in]  x         Component X start position
 * \param[in]  y         Component Y start position
 * \param[out] pattable  The pattern table
 */
void nsk_pattable_convfrompng(
    const struct nsk_type_pngimage *image,
    size_t x,
    size_t y,
    struct nsk_type_pattable *pattable
);

#endif
