#include <stddef.h>
#include <string.h>

#include "../types/nsk_type_image.h"
#include "../utils/nsk_util_malloc.h"
#include "../utils/strings/nsk_strings_strdup.h"
#include "../utils/log/nsk_log_err.h"
#include "../utils/log/nsk_log_inf.h"

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
) {
    png_byte **bytes = png_get_rows(png_ptr, info_ptr);

    struct nsk_image *image = nsk_util_malloc(sizeof(*image));
    image->width  = png_get_image_width(png_ptr, info_ptr);
    image->height = png_get_image_height(png_ptr, info_ptr);
    image->filename = nsk_strings_strdup(filename);
    image->data   = nsk_util_malloc(sizeof(*image->data) * image->height);

    for (size_t h = 0; h < image->height; h++) {
        image->data[h] = nsk_util_malloc(sizeof(**image->data) * image->width);
        memcpy(image->data[h], bytes[h], image->width * sizeof(**image->data));
    }

    return image;
}

/*!
 * \brief  Creates empty image
 *
 * \param[in] width   The width
 * \param[in] height  The height
 * \return Allocated image
 */
struct nsk_image *nsk_image_empty(size_t width, size_t height) {
    struct nsk_image *image = nsk_util_malloc(sizeof(*image));
    image->width  = width;
    image->height = height;
    image->filename = nsk_strings_strdup("//Empty");
    image->data   = nsk_util_malloc(sizeof(*image->data) * image->height);

    for (size_t h = 0; h < image->height; h++) {
        image->data[h] = nsk_util_malloc(sizeof(**image->data) * image->width);
        memset(image->data[h], 0xff, image->width * sizeof(**image->data));
    }
    return image;
}

/*!
 * \brief  Checks if the images matches provided sizes
 *
 * \param[in] image   The image
 * \param[in] width   The width
 * \param[in] height  The height
 */
void nsk_image_measure(struct nsk_image *image, size_t width, size_t height) {
    if (image->width  != width ||
        image->height != height) {

        nsk_err(
            "Image \"%s\" doesn't match required size: "
                "should be %zux%zu instead of %zux%zu\n",
            image->filename,
            width,
            height,
            image->width,
            image->height
        );
    }
}

/*!
 * \brief  Combines the target and component images at X,Y
 *
 * \param[in, out] target  The target
 * \param[in] component    The component
 * \param[in] x            X position
 * \param[in] y            Y position
 */
void nsk_image_combine(
    struct nsk_image       *target,
    const struct nsk_image *component,
    size_t x,
    size_t y
) {
    for (size_t iy; iy < component->height; iy++) {
        for (size_t ix; ix < component->width; ix++) {
            target->data[y + iy][x + ix].raw = component->data[iy][ix].raw;
        }
    }
}

/*!
 * \brief  Frees the nsk_image data
 *
 * \param[in, out]  image
 */
void nsk_image_free(struct nsk_image *image) {
    if (!image) {
        return;
    }

    for (size_t h = 0; h < image->height; h++) {
        nsk_util_free(image->data[h]);
    }
    nsk_util_free(image->data);
    nsk_util_free(image->filename);
    nsk_util_free(image);
}