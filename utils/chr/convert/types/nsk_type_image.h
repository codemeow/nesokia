#ifndef NSK_TYPE_IMAGE
#define NSK_TYPE_IMAGE

#include <stddef.h>
#include <stdint.h>

#include <libpng/png.h>

/*!
 * \brief  Single pixel data
 */
union nsk_pixel {
    uint32_t raw;
    struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };
};

/*!
 * \brief  Image data
 */
struct nsk_image {
    char             *filename; /*!< Original filename      */
    size_t            width;    /*!< Image width            */
    size_t            height;   /*!< Image height           */
    union nsk_pixel **data;     /*!< Image pixels as [y][x] */
};

/*!
 * \brief  Creates image in local format
 *
 * \param[in] filename  Original filename
 * \param[in] png_ptr   The pnglib main image pointer
 * \param[in] info_ptr  The pnglib info image pointer
 * \return Allocated image
 */
struct nsk_image *nsk_image_create(
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
struct nsk_image *nsk_image_empty(size_t width, size_t height);

/*!
 * \brief  Checks if the images matches provided sizes
 *
 * \param[in] image   The image
 * \param[in] width   The width
 * \param[in] height  The height
 */
void nsk_image_measure(struct nsk_image *image, size_t width, size_t height);

/*!
 * \brief  Combines the target and component images at X,Y
 *
 * \param[in, out] target  The target
 * \param[in] component    The component
 * \param[in] x            X position
 * \param[in] y            Y position
 */
void nsk_image_combine(
    struct nsk_image *target,
    const struct nsk_image *component,
    size_t x,
    size_t y
);

/*!
 * \brief  Frees the nsk_image data
 *
 * \param[in, out]  image
 */
void nsk_image_free(struct nsk_image *image);

#endif