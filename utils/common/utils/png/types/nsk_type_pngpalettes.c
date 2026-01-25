#include <stdlib.h>

#include "../../png/types/nsk_type_pngpalettes.h"
#include "../../io/nsk_io_fullpath.h"
#include "../../log/nsk_log_err.h"

/*!
 * \brief  Positioning of the global palette in the template image
 */
enum nsk_palettes_templpos {
    NSK_PALETTESTEMPLPOS_X = 16, /*!< X position data start */
    NSK_PALETTESTEMPLPOS_Y = 24  /*!< Y position data start */
};

/*!
 * \brief  The colors cells sizes
 */
enum nsk_palettes_cell {
    NSK_PALETTESCELL_WIDTH  = 8, /*!< Width of the colors cells in pixels  */
    NSK_PALETTESCELL_HEIGHT = 8, /*!< Height of the colors cells in pixels */
};

/*!
 * \brief  Padding of data start in each cell
 */
enum nsk_palettes_pad {
    NSK_PALETTESPAD_X = 1, /*!< X padding of data of each cell */
    NSK_PALETTESPAD_Y = 1  /*!< Y padding of data of each cell */
};

/*!
 * \brief  Template size
 */
enum nsk_palettes_size {
    NSK_PALETTESSIZE_WIDTH = 152, /*!< Width of the template  */
    NSK_PALETTESSIZE_HEIGHT = 64  /*!< Height of the template */
};

/*!
 * \brief  Reads the palettes from Nesokia PNG from selected position
 *
 * \param[in] image     The source image
 * \param[in] startx    The startx
 * \param[in] starty    The starty
 * \return Local palettes
 */
struct nsk_type_palettes _palettes_readpng(
    const struct nsk_type_pngimage *image,
    size_t startx,
    size_t starty
) {
    struct nsk_type_palettes palettes = { 0 };

    for (size_t p = 0; p < NSK_PLANES_COUNT; p++) {
        for (size_t g = 0; g < NSK_PALETTESIZE_GROUPS; g++) {
            for (size_t c = 0; c < NSK_PALETTESIZE_COLORS; c++) {
                const size_t posy =
                    starty +
                    p * NSK_PALETTESCELL_HEIGHT +
                    NSK_PALETTESPAD_Y;
                const size_t posx =
                    startx +
                    (g * NSK_PALETTESIZE_COLORS + c) * NSK_PALETTESCELL_WIDTH +
                    NSK_PALETTESPAD_X;

                palettes.plane[p].group[g].color[c] =
                    image->data[posy][posx];
            }
        }

        palettes.plane[p].init.colors = true;
    }

    return palettes;
}

/*!
 * \brief  Reads local palettes from Nesokia PNG component
 *
 * \param[in] filename  The filename
 * \return Local palettes
 */
struct nsk_type_palettes nsk_palettes_readpng(
    const char *filename
) {
    nsk_auto_pifree struct nsk_type_pngimage *image = nsk_pngimage_read(
        filename
    );

    if (image->width  != NSK_PALETTESSIZE_WIDTH ||
        image->height != NSK_PALETTESSIZE_HEIGHT) {
        nsk_err(
            "Provided file \"%s\" is not a palettes template component",
            filename
        );
        exit(EXIT_FAILURE);
    }

    return _palettes_readpng(
        image,
        NSK_PALETTESTEMPLPOS_X,
        NSK_PALETTESTEMPLPOS_Y
    );
}

/*!
 * \brief  Converts local palettes into composite component
 *
 * \param[in] palettes  The palettes
 * \return Nesokia PNG component image
 */
struct nsk_type_pngimage *nsk_palettes_convtopng(
    const struct nsk_type_palettes *palettes
) {
    static const char template_path[] = "./templates/png/template-palettes.png";
    const char *template_fullpath = nsk_io_fullpath(template_path);
    struct nsk_type_pngimage *image = nsk_pngimage_read(
        template_fullpath
    );

    for (size_t p = 0; p < NSK_PLANES_COUNT; p++) {
        nsk_palette_validate_colors(&palettes->plane[p]);

        for (size_t g = 0; g < NSK_PALETTESIZE_GROUPS; g++) {
            for (size_t c = 0; c < NSK_PALETTESIZE_COLORS; c++) {
                const size_t posy =
                    NSK_PALETTESTEMPLPOS_Y +
                    p * NSK_PALETTESCELL_HEIGHT +
                    NSK_PALETTESPAD_Y;
                const size_t posx =
                    NSK_PALETTESTEMPLPOS_X +
                    (g * NSK_PALETTESIZE_COLORS + c) * NSK_PALETTESCELL_WIDTH +
                    NSK_PALETTESPAD_X;

                const union nsk_type_color4 color =
                    palettes->plane[p].group[g].color[c];

                nsk_pngimage_cellset(
                    image,
                    posx,
                    posy,
                    NSK_PALETTESCELL_WIDTH  - NSK_PALETTESPAD_X,
                    NSK_PALETTESCELL_HEIGHT - NSK_PALETTESPAD_Y,
                    color
                );
            }
        }
    }

    return image;
}

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
) {
    *palettes = _palettes_readpng(
        image,
        x + NSK_PALETTESTEMPLPOS_X,
        y + NSK_PALETTESTEMPLPOS_Y
    );
}
