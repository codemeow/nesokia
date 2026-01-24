#if defined(NSK_MODULE_COMPOSE) && defined(NSK_MODULE_PNG)

#include <string.h>
#include <libpng/png.h>

#include "../../png/types/nsk_type_pngimage.h"
#include "../../io/nsk_io_fopen.h"
#include "../../log/nsk_log_err.h"
#include "../../nsk_util_cleanup.h"
#include "../../nsk_util_malloc.h"
#include "../../strings/nsk_strings_strdup.h"

/*!
 * Size of the PNG signature
 */
#define NSK_PNG_SIG_SIZE (8)

/*!
 * \brief Autocleaner for nsk_type_pngimage
 *
 * \param[in,out]  image  The image
 */
void _nsk_auto_pifree(struct nsk_type_pngimage **image) {
    if (!image || !*image) {
        return;
    }

    nsk_pngimage_free(*image);
}

/*!
 * \brief  Opens the file and checks it
 *
 * \param[in] filename  The filename
 * \return Opened descriptor
 */
static FILE *_pngimage_openfile(const char *filename) {
    FILE *file = nsk_io_fopen(filename, "rb");
    if (!file) {
        nsk_err(
            "Cannot open file \"%s\"\n",
            filename
        );
        exit(EXIT_FAILURE);
    }
    return file;
}

/*!
 * \brief  Validates the PNG signature of the file
 *
 * \param[in,out]  file  The file
 * \param[in] filename  Original filename
 */
static void _pngimage_validate(FILE *file, const char *filename) {
    uint8_t sig[NSK_PNG_SIG_SIZE];
    if (fread(sig, sizeof(sig), 1, file) != 1) {
        nsk_err(
            "Cannot read PNG signature from the \"%s\" file",
            filename
        );
        exit(EXIT_FAILURE);
    }
    if (png_sig_cmp(sig, 0, NSK_PNG_SIG_SIZE) != 0) {
        nsk_err(
            "File \"%s\" is not a PNG file\n",
            filename
        );
        exit(EXIT_FAILURE);
    }
}

/*!
 * \brief  Creates new PNG struct pointer
 *
 * \return The png structp.
 */
static png_structp _pngimage_create_png(void) {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        nsk_err(
            "Out of memory while creating PNG ptr\n"
        );
        exit(EXIT_FAILURE);
    }
    return png_ptr;
}

/*!
 * \brief  Creates PNG info struct
 *
 * \param[in] png_ptr  The png pointer
 * \return    The png infop.
 */
static png_infop _pngimage_create_info(png_structp png_ptr) {
    png_infop   info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        nsk_err(
            "Out of memory while creating PNG info\n"
        );
        exit(EXIT_FAILURE);
    }
    return info_ptr;
}

/*!
 * \brief  Converts the PNG image to RGBA format
 *
 * \param[in] png_ptr   The pnglib main image pointer
 * \param[in] info_ptr  The pnglib info image pointer
 */
static void _pngimage_convert2rgba(png_structp png_ptr, png_infop info_ptr) {
    uint8_t  bits   = png_get_bit_depth(png_ptr, info_ptr);
    uint8_t  color  = png_get_color_type(png_ptr, info_ptr);

    if (color == PNG_COLOR_TYPE_PALETTE) {
       png_set_palette_to_rgb(png_ptr);
    }

    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
       png_set_tRNS_to_alpha(png_ptr);
    }

    png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);

    if (color == PNG_COLOR_TYPE_GRAY) {
        png_set_gray_to_rgb(png_ptr);
    }

    if (color == PNG_COLOR_TYPE_GRAY && bits < 8) {
       png_set_expand_gray_1_2_4_to_8(png_ptr);
    }

    if (bits == 16) {
#if PNG_LIBPNG_VER >= 10504
       png_set_scale_16(png_ptr);
#else
       png_set_strip_16(png_ptr);
#endif
    }
}

/*!
 * \brief  Creates image in local format
 *
 * \param[in] filename  Original filename
 * \param[in] png_ptr   The pnglib main image pointer
 * \param[in] info_ptr  The pnglib info image pointer
 * \return Allocated image
 */
struct nsk_type_pngimage *_pngimage_create(
    const char *filename,
    png_structp png_ptr,
    png_infop info_ptr
) {
    png_byte **bytes = png_get_rows(png_ptr, info_ptr);

    struct nsk_type_pngimage *image = nsk_util_malloc(sizeof(*image));
    image->width    = png_get_image_width (png_ptr, info_ptr);
    image->height   = png_get_image_height(png_ptr, info_ptr);
    image->filename = nsk_strings_strdup(filename);
    image->data     = nsk_util_malloc(sizeof(*image->data) * image->height);

    for (size_t h = 0; h < image->height; h++) {
        image->data[h] = nsk_util_malloc(sizeof(**image->data) * image->width);
        memcpy(image->data[h], bytes[h], image->width * sizeof(**image->data));
    }

    return image;
}

/*!
 * \brief  Creates image in local format
 *
 * \param[in] filename  Original filename
 * \return Allocated image
 */
struct nsk_type_pngimage *nsk_pngimage_read(
    const char *filename
) {
    png_structp png_ptr;
    png_infop   info_ptr;
    struct nsk_type_pngimage *image;

    nsk_auto_fclose FILE *file = _pngimage_openfile(filename);
    _pngimage_validate(file, filename);
    png_ptr  = _pngimage_create_png();
    info_ptr = _pngimage_create_info(png_ptr);

    /* Cannot be legally exported to static function */
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        nsk_err(
            "setjmp unexpectedly failed\n"
        );
        exit(EXIT_FAILURE);
    }

    png_init_io(png_ptr, file);
    png_set_sig_bytes(png_ptr, NSK_PNG_SIG_SIZE);
    _pngimage_convert2rgba(png_ptr, info_ptr);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    image = _pngimage_create(filename, png_ptr, info_ptr);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return image;
}

/*!
 * \brief  Writes image data to PNG
 *
 * \param[in] png_ptr  The png pointer
 * \param[in] image    The image
 */
static void _pngimage_writedata(
    png_structp png_ptr,
    const struct nsk_type_pngimage *image
) {
    nsk_auto_free png_bytep row = nsk_util_malloc(image->width * 3);
    for (size_t y = 0; y < image->height; ++y) {
        for (size_t x = 0; x < image->width; ++x) {
            row[x*3 + 0] = image->data[y][x].r;
            row[x*3 + 1] = image->data[y][x].g;
            row[x*3 + 2] = image->data[y][x].b;
        }
        png_write_row(png_ptr, row);
    }
}

/*!
 * \brief  Saves the provided image to file
 *
 * \param[in] image     The image
 * \param[in] filename  The filename
 */
void nsk_pngimage_write(
    const struct nsk_type_pngimage *image,
    const char *filename
) {
    png_structp png_ptr;
    png_infop info_ptr;
    nsk_auto_fclose FILE *file = nsk_io_fopen(filename, "wb");
    if (!file) {
        nsk_err(
            "Cannot open file \"%s\"\n",
            filename
        );
        exit(EXIT_FAILURE);
    }

    png_ptr  = _pngimage_create_png();
    info_ptr = _pngimage_create_info(png_ptr);

    /* Cannot be legally exported to static function */
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        nsk_err(
            "setjmp unexpectedly failed\n"
        );
        exit(EXIT_FAILURE);
    }

    png_init_io(png_ptr, file);
    png_set_IHDR(
        png_ptr, info_ptr,
        image->width,
        image->height,
        8,
        PNG_COLOR_TYPE_RGB,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png_ptr, info_ptr);
    _pngimage_writedata(png_ptr, image);
    png_write_end(png_ptr, NULL);
    png_destroy_write_struct(&png_ptr, &info_ptr);
}

/*!
 * \brief  Creates empty image
 *
 * \param[in] width   The width
 * \param[in] height  The height
 * \return Allocated image
 */
struct nsk_type_pngimage *nsk_pngimage_empty(
    size_t width,
    size_t height
) {
    struct nsk_type_pngimage *image = nsk_util_malloc(sizeof(*image));
    image->width    = width;
    image->height   = height;
    image->filename = nsk_strings_strdup("//Empty");
    image->data     = nsk_util_malloc(sizeof(*image->data) * image->height);

    for (size_t h = 0; h < image->height; h++) {
        image->data[h] = nsk_util_malloc(sizeof(**image->data) * image->width);
        memset(image->data[h], 0xff, image->width * sizeof(**image->data));
    }
    return image;
}

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
) {
    // @todo to memcpy if it shows that this one is slow
    for (size_t iy = 0; iy < component->height; iy++) {
        for (size_t ix = 0; ix < component->width; ix++) {
            target->data[y + iy][x + ix].raw = component->data[iy][ix].raw;
        }
    }
}

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
    size_t startx,
    size_t starty,
    size_t width,
    size_t height,
    union nsk_type_color4 color
) {
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            image->data[starty + y][startx + x] = color;
        }
    }
}

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
    size_t startx,
    size_t starty,
    size_t width,
    size_t height
) {
    const union nsk_type_color4 color_back = {
        .r = 0,
        .g = 0,
        .b = 0
    };
    const union nsk_type_color4 color_front = {
        .r = 236,
        .g = 238,
        .b = 236
    };

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            if ((x != 0 && y != 0) &&
                (x != width - 1 && y != height - 1) &&
                ((x + y) % 2 == 0)
            ) {
                image->data[starty + y][startx + x] = color_front;
            } else {
                image->data[starty + y][startx + x] = color_back;
            }
        }
    }
}

/*!
 * \brief  Frees the nsk_type_image data
 *
 * \param[in, out]  image
 */
void nsk_pngimage_free(
    struct nsk_type_pngimage *image
) {
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

#endif