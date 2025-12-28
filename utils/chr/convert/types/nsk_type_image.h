#ifndef NSK_TYPE_IMAGE
#define NSK_TYPE_IMAGE

#include <stddef.h>

#include <libpng/png.h>

#include "../types/nsk_type_pixel.h"

/*!
 * \brief  Image data
 */
struct nsk_type_image {
    char             *filename; /*!< Original filename      */
    size_t            width;    /*!< Image width            */
    size_t            height;   /*!< Image height           */
    union nsk_type_pixel **data;     /*!< Image pixels as [y][x] */
};

/*!
 * \brief  Creates image in local format
 *
 * \param[in] filename  Original filename
 * \param[in] png_ptr   The pnglib main image pointer
 * \param[in] info_ptr  The pnglib info image pointer
 * \return Allocated image
 */
struct nsk_type_image *nsk_image_create(
    const char *filename,
    png_structp png_ptr,
    png_infop info_ptr
);

/*!
 * \brief  Creates empty image
 *
 * \param[in] width   The width
 * \param[in] height  The height
 * \return Allocated image
 */
struct nsk_type_image *nsk_image_empty(size_t width, size_t height);

/*!
 * \brief  Checks if the images matches provided sizes
 *
 * \param[in] image   The image
 * \param[in] width   The width
 * \param[in] height  The height
 */
void nsk_image_measure(struct nsk_type_image *image, size_t width, size_t height);

/*!
 * \brief  Combines the target and component images at X,Y
 *
 * \param[in, out] target  The target
 * \param[in] component    The component
 * \param[in] x            X position
 * \param[in] y            Y position
 */
void nsk_image_combine(
    struct nsk_type_image *target,
    const struct nsk_type_image *component,
    size_t x,
    size_t y
);

/*!
 * \brief  Frees the nsk_type_image data
 *
 * \param[in, out]  image
 */
void nsk_image_free(struct nsk_type_image *image);

/*!
 * \brief  Reads the image from the input file/files
 */
void nsk_image_read(void);

/*!
 * \brief  Validates the image
 */
void nsk_image_validate(void);

#endif