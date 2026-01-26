#include <stdlib.h>

#include "../../png/types/nsk_type_pngpattable.h"
#include "../../png/types/nsk_type_pngtile.h"
#include "../../log/nsk_log_err.h"
#include "../../io/nsk_io_fullpath.h"

/*!
 * \brief  Positioning of the global palette in the template image
 */
enum nsk_pattable_templpos {
    NSK_PATTABLETEMPLPOS_X = 16, /*!< X position data start */
    NSK_PATTABLETEMPLPOS_Y = 24  /*!< Y position data start */
};

/*!
 * \brief  The pattern table cells sizes
 */
enum nsk_pattable_cell {
    NSK_PATTABLECELL_WIDTH  = 8, /*!< Width of the cells in pixels  */
    NSK_PATTABLECELL_HEIGHT = 8, /*!< Height of the cells in pixels */
};

/*!
 * \brief  Template size
 */
enum nsk_pattable_size {
    NSK_PATTABLESIZE_WIDTH  = 152,  /*!< Width of the template  */
    NSK_PATTABLESIZE_HEIGHT = 160  /*!< Height of the template */
};

/*!
 * \brief  Reads the pattern table from Nesokia PNG from selected position
 *
 * \param[in] image     The source image
 * \param[in] startx    The startx
 * \param[in] starty    The starty
 * \return Pattern table
 */
struct nsk_type_pattable _pattable_readpng(
    const struct nsk_type_pngimage *image,
    size_t startx,
    size_t starty
) {
    struct nsk_type_pattable table = { 0 };

    for (size_t y = 0; y < NSK_PATTABLETABLE_HEIGHT; y++) {
        for (size_t x = 0; x < NSK_PATTABLETABLE_WIDTH; x++) {
            nsk_tile_readpng(
                image,
                startx + x * NSK_PATTABLECELL_WIDTH,
                starty + y * NSK_PATTABLECELL_HEIGHT,
                &table.tile[y][x]
            );
        }
    }

    return table;
}

/*!
 * \brief  Reads pattern table from Nesokia PNG component
 *
 * \param[in] filename  The filename
 * \return Local palettes
 */
struct nsk_type_pattable nsk_pattable_readpng(
    const char *filename
) {
    nsk_auto_pifree struct nsk_type_pngimage *image = nsk_pngimage_read(
        filename
    );

    if (image->width  != NSK_PATTABLESIZE_WIDTH ||
        image->height != NSK_PATTABLESIZE_HEIGHT) {
        nsk_err(
            "Provided file \"%s\" is not a pattern table template component",
            filename
        );
        exit(EXIT_FAILURE);
    }

    return _pattable_readpng(
        image,
        NSK_PATTABLETEMPLPOS_X,
        NSK_PATTABLETEMPLPOS_Y
    );
}

/*!
 * \brief  Converts pattern table into composite component
 *
 * \param[in] pattable  The pattern table
 * \return Nesokia PNG component image
 */
struct nsk_type_pngimage *nsk_pattable_convtopng(
    const struct nsk_type_pattable *pattable
) {
    static const char *template_path[] = {
        [NSK_PATTABLEADDR_0000] = "./templates/png/template-left.png",
        [NSK_PATTABLEADDR_1000] = "./templates/png/template-right.png",
    };

    nsk_pattable_validate_address(pattable);

    const char *template_fullpath = nsk_io_fullpath(
        template_path[pattable->address]
    );
    struct nsk_type_pngimage *image = nsk_pngimage_read(
        template_fullpath
    );

    for (size_t y = 0; y < NSK_PATTABLETABLE_HEIGHT; y++) {
        for (size_t x = 0; x < NSK_PATTABLETABLE_WIDTH; x++) {
            nsk_tile_convtopng(
                &pattable->tile[y][x],
                NSK_PATTABLETEMPLPOS_X + x * NSK_PATTABLECELL_WIDTH,
                NSK_PATTABLETEMPLPOS_Y + y * NSK_PATTABLECELL_HEIGHT,
                image
            );
        }
    }

    return image;
}

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
) {
    *pattable = _pattable_readpng(
        image,
        x + NSK_PATTABLETEMPLPOS_X,
        y + NSK_PATTABLETEMPLPOS_Y
    );
}
