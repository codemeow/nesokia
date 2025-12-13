#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>

#include <libpng/png.h>

#include "../png/nsk_png_read.h"

#include "../types/nsk_type_image.h"
#include "../utils/io/nsk_io_fopen.h"
#include "../utils/log/nsk_log_err.h"
#include "../utils/nsk_util_cleanup.h"

/*!
 * \brief  Converts the PNG image to RGBA format
 *
 * \param[in] png_ptr   The pnglib main image pointer
 * \param[in] info_ptr  The pnglib info image pointer
 */
static void _png_convert2rgba(png_structp png_ptr, png_infop info_ptr) {
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
 * \brief  Reads the selected PNG file into the localimage format
 *
 * \param[in] filename  The filename
 * \return Allocated image
 */
struct nsk_image *nsk_png_read(const char *filename) {
    png_structp png_ptr;
    png_infop   info_ptr;

    nsk_auto_fclose FILE *file = nsk_io_fopen(filename, "rb");
    if (!file) {
        nsk_err(
            "Cannot open file \"%s\"\n",
            filename
        );
        exit(EXIT_FAILURE);
    }

    uint8_t sig[8];
    fread(sig, 1, sizeof(sig), file);
    if (!png_check_sig(sig, sizeof(sig))) {
        nsk_err(
            "File \"%s\" is not a PNG file\n",
            filename
        );
        exit(EXIT_FAILURE);
    }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        nsk_err(
            "Out of memory¹ while reading \"%s\" PNG file\n",
            filename
        );
        exit(EXIT_FAILURE);
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        nsk_err(
            "Out of memory² while reading \"%s\" PNG file\n",
            filename
        );
        exit(EXIT_FAILURE);
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        nsk_err(
            "setjmp unexpectedly failed\n"
        );
        exit(EXIT_FAILURE);
    }

    png_init_io(png_ptr, file);

    png_set_sig_bytes(png_ptr, sizeof(sig));

    _png_convert2rgba(png_ptr, info_ptr);

    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    struct nsk_image *image = nsk_image_create(filename, png_ptr, info_ptr);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return image;
}