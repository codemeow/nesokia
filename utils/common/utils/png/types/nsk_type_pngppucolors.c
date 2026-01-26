#include <stdlib.h>

#include "../../png/types/nsk_type_pngppucolors.h"
#include "../../types/nsk_type_color4.h"
#include "../../io/nsk_io_fullpath.h"
#include "../../log/nsk_log_err.h"

/*!
 * \brief  Positioning of the global palette in the template image
 */
enum nsk_ppucolors_templpos {
    NSK_PPUCOLORSTEMPLPOS_X = 16, /*!< X position data start */
    NSK_PPUCOLORSTEMPLPOS_Y = 24  /*!< Y position data start */
};

/*!
 * \brief  The colors cells sizes
 */
enum nsk_ppucolors_cell {
    NSK_PPUCOLORSCELL_WIDTH  = 8, /*!< Width of the colors cells in pixels  */
    NSK_PPUCOLORSCELL_HEIGHT = 8, /*!< Height of the colors cells in pixels */
};

/*!
 * \brief  Template size
 */
enum nsk_ppucolors_size {
    NSK_PPUCOLORSSIZE_WIDTH = 152, /*!< Width of the template  */
    NSK_PPUCOLORSSIZE_HEIGHT = 64  /*!< Height of the template */
};

/*!
 * \brief  Checks if this color is not marked as invalid
 *
 * \note The valid (true) color has the whole cell single-colored
 *       The invalid (false) color has any markings of other color in the cell
 *
 * \param[in] image   The parent image
 * \param[in] color   The color
 * \param[in] startx  X position of the cell in the image
 * \param[in] starty  Y position of the cell in the image
 * \return True if this cell is allowed to use
 */
static bool _color_isallowed(
    const struct nsk_type_pngimage *image,
    const union nsk_type_color4 *color,
    size_t startx,
    size_t starty
) {
    for (size_t h = 0; h < NSK_PPUCOLORSCELL_HEIGHT; h++) {
        for (size_t w = 0; w < NSK_PPUCOLORSCELL_WIDTH; w++) {
            const size_t posy = starty + h;
            const size_t posx = startx + w;

            if (image->data[posy][posx].raw != color->raw) {
                return false;
            }
        }
    }
    return true;
}

/*!
 * \brief  Reads the PPU colors from Nesokia PNG from selected position
 *
 * \param[in] image     The source image
 * \param[in] startx    The startx
 * \param[in] starty    The starty
 * \return PPU colors
 */
struct nsk_type_ppucolors _ppucolors_readpng(
    const struct nsk_type_pngimage *image,
    size_t startx,
    size_t starty
) {
    struct nsk_type_ppucolors colors = { 0 };

    for (size_t y = 0; y < NSK_PPUCOLORSTABLE_HEIGHT; y++) {
        for (size_t x = 0; x < NSK_PPUCOLORSTABLE_WIDTH; x++) {
            const size_t index = y * NSK_PPUCOLORSTABLE_WIDTH + x;
            const size_t posy  =
                starty + y * NSK_PPUCOLORSCELL_HEIGHT;
            const size_t posx  =
                startx + x * NSK_PPUCOLORSCELL_WIDTH;

            union nsk_type_color4 color = image->data[posy][posx];

            colors.colors [index] = color;
            colors.allowed[index] = _color_isallowed(
                image,
                &color,
                posx,
                posy
            );
        }
    }

    return colors;
}

/*!
 * \brief  Reads the PPU colors from Nesokia PNG component
 *
 * \param[in] filename  The filename
 * \return PPU colors
 */
struct nsk_type_ppucolors nsk_ppucolors_readpng(
    const char *filename
) {
    nsk_auto_pifree struct nsk_type_pngimage *image = nsk_pngimage_read(
        filename
    );

    if (image->width  != NSK_PPUCOLORSSIZE_WIDTH ||
        image->height != NSK_PPUCOLORSSIZE_HEIGHT) {
        nsk_err(
            "Provided file \"%s\" is not a palettes template component",
            filename
        );
        exit(EXIT_FAILURE);
    }

    return _ppucolors_readpng(
        image,
        NSK_PPUCOLORSTEMPLPOS_X,
        NSK_PPUCOLORSTEMPLPOS_Y
    );
}

/*!
 * \brief  Converts PPU colors into composite component
 *
 * \param[in] colors  The colors
 * \return Nesokia PNG component image
 */
struct nsk_type_pngimage *nsk_ppucolors_convtopng(
    const struct nsk_type_ppucolors *colors
) {
    static const char template_path[] = "./templates/png/template-colors.png";
    const char *template_fullpath = nsk_io_fullpath(template_path);
    struct nsk_type_pngimage *image = nsk_pngimage_read(
        template_fullpath
    );

    for (size_t y = 0; y < NSK_PPUCOLORSTABLE_HEIGHT; y++) {
        for (size_t x = 0; x < NSK_PPUCOLORSTABLE_WIDTH; x++) {
            const size_t index = y * NSK_PPUCOLORSTABLE_WIDTH + x;
            const size_t posy  =
                NSK_PPUCOLORSTEMPLPOS_Y + y * NSK_PPUCOLORSCELL_HEIGHT;
            const size_t posx  =
                NSK_PPUCOLORSTEMPLPOS_X + x * NSK_PPUCOLORSCELL_WIDTH;

            if (colors->allowed[index]) {
                const union nsk_type_color4 color = colors->colors[index];

                nsk_pngimage_cellset(
                    image,
                    posx,
                    posy,
                    NSK_PPUCOLORSCELL_WIDTH,
                    NSK_PPUCOLORSCELL_HEIGHT,
                    color
                );

            } else {
                nsk_pngimage_cellmark(
                    image,
                    posx,
                    posy,
                    NSK_PPUCOLORSCELL_WIDTH,
                    NSK_PPUCOLORSCELL_HEIGHT
                );
            }
        }
    }

    return image;
}

/*!
 * \brief  Reads the PPU colors from the PNG template
 *
 * \param[in]  image   The image
 * \param[in]  x       Template start X position
 * \param[in]  y       Template start Y position
 * \param[out] colors  The colors
 */
void nsk_ppucolors_convfrompng(
    const struct nsk_type_pngimage *image,
    size_t x,
    size_t y,
    struct nsk_type_ppucolors *colors
) {
    *colors = _ppucolors_readpng(
        image,
        x + NSK_PPUCOLORSTEMPLPOS_X,
        y + NSK_PPUCOLORSTEMPLPOS_Y
    );
}
