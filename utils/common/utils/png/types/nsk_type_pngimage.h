#ifndef NSK_TYPE_PNGIMAGE
#define NSK_TYPE_PNGIMAGE

#include <stddef.h>
#include "../../types/nsk_type_color4.h"

/*!
 * \brief  Image data
 */
struct nsk_type_pngimage {
    char                     *filename; /*!< Original filename      */
    size_t                    width;    /*!< Image width            */
    size_t                    height;   /*!< Image height           */
    union nsk_type_color4   **data;     /*!< Image pixels as [y][x] */
};

/*!
 * Automatically free the allocated memory on exit
 */
#define nsk_auto_pifree __attribute__((cleanup(_nsk_auto_pifree)))

/*!
 * \brief Autocleaner for nsk_type_pngimage
 *
 * \param[in,out]  image  The image
 */
void _nsk_auto_pifree(struct nsk_type_pngimage **image);

/*!
 * \brief  Creates image in local format
 *
 * \param[in] filename  Original filename
 * \return Allocated image
 */
struct nsk_type_pngimage *nsk_pngimage_read(const char *filename);

/*!
 * \brief  Saves the provided image to file
 *
 * \param[in] image     The image
 * \param[in] filename  The filename
 */
void nsk_pngimage_write(
    const struct nsk_type_pngimage *image,
    const char *filename
);

/*!
 * \brief  Creates empty image
 *
 * \param[in] width   The width
 * \param[in] height  The height
 * \return Allocated image
 */
struct nsk_type_pngimage *nsk_pngimage_empty(size_t width, size_t height);

/*!
 * \brief  Combines the target and component images at X,Y
 *
 * \param[in, out] target  The target
 * \param[in] component    The component
 * \param[in] x            X position
 * \param[in] y            Y position
 */
void nsk_pngimage_combine(
    struct nsk_type_pngimage *target,
    const struct nsk_type_pngimage *component,
    size_t x,
    size_t y
);

/*!
 * \brief  Fills the selected cell with a single color
 *
 * \param[in,out]   image   The image
 * \param[in]       x       Cell x position
 * \param[in]       y       Cell y position
 * \param[in]       width   Cell width
 * \param[in]       height  Cell height
 * \param[in]       color   The color
 */
void nsk_pngimage_cellset(
    struct nsk_type_pngimage *image,
    size_t x,
    size_t y,
    size_t width,
    size_t height,
    union nsk_type_color4 color
);

/*!
 * \brief  Marks the selected cell as invalid
 *
 * \param[in,out]   image   The image
 * \param[in]       x       Cell x position
 * \param[in]       y       Cell y position
 * \param[in]       width   Cell width
 * \param[in]       height  Cell height
 */
void nsk_pngimage_cellmark(
    struct nsk_type_pngimage *image,
    size_t x,
    size_t y,
    size_t width,
    size_t height
);

/*!
 * \brief  Frees the nsk_type_image data
 *
 * \param[in, out]  image
 */
void nsk_pngimage_free(struct nsk_type_pngimage *image);

#endif