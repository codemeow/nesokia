#if defined(NSK_MODULE_COMPOSE) && defined(NSK_MODULE_PNG)

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../../png/types/nsk_type_pngcompose.h"
#include "../../nsk_util_size.h"
#include "../../png/types/nsk_type_pngppucolors.h"
#include "../../png/types/nsk_type_pngpalettes.h"
#include "../../png/types/nsk_type_pngpattable.h"
#include "../../log/nsk_log_err.h"

/*!
 * \brief  Sizes of components
 */
enum nsk_compose_size {
    NSK_COMPOSESIZE_FULL_WIDTH      = 328, /*!< Width of full image  */
    NSK_COMPOSESIZE_FULL_HEIGHT     = 248, /*!< Height of full image */
};

/*!
 * \brief Positions of components
 */
enum nsk_compose_pos {
    NSK_COMPOSEPOS_COLORS_X   = 8,    /*!< Global palette X pos   */
    NSK_COMPOSEPOS_COLORS_Y   = 176,  /*!< Global palette Y pos   */
    NSK_COMPOSEPOS_PALETTES_X = 168,  /*!< Local palettes X pos   */
    NSK_COMPOSEPOS_PALETTES_Y = 176,  /*!< Local palettes Y pos   */
    NSK_COMPOSEPOS_LEFT_X     = 8,    /*!< Left pat.table X pos   */
    NSK_COMPOSEPOS_LEFT_Y     = 8,    /*!< Left pat.table Y pos   */
    NSK_COMPOSEPOS_RIGHT_X    = 168,  /*!< Right pat.table X pos  */
    NSK_COMPOSEPOS_RIGHT_Y    = 8,    /*!< Right pat.table Y pos  */
};

/*!
 * \brief  List of components in compose mode
 */
enum nsk_compose_components {
    NSK_COMPOSECOMP_LEFT,      /*!< Left  pattern table     */
    NSK_COMPOSECOMP_RIGHT,     /*!< Right pattern table     */
    NSK_COMPOSECOMP_PPUCOLORS, /*!< PPU colors table        */
    NSK_COMPOSECOMP_PALETTES,  /*!< Palettes list           */

    NSK_COMPOSECOMPS_COUNT     /*!< Number of components    */
};

/*!
 * \brief  Converts PPU colors to image
 *
 * \param[in]  object  PPU colors
 * \return New image
 */
static struct nsk_type_pngimage *_convert_ppucolors_o2i(
    const void *object
) {
    const struct nsk_type_ppucolors *colors = object;
    return nsk_ppucolors_convtopng(colors);
}

/*!
 * \brief  Converts palettes to image
 *
 * \param[in]  object  palettes
 * \return New image
 */
static struct nsk_type_pngimage *_convert_palettes_o2i(
    const void *object
) {
    const struct nsk_type_palettes *palettes = object;
    return nsk_palettes_convtopng(palettes);
}

/*!
 * \brief  Converts pattern table to image
 *
 * \param[in]  object  PPU colors
 * \return New image
 */
static struct nsk_type_pngimage *_convert_pattable_o2i(
    const void *object
) {
    const struct nsk_type_pattable *pattable = object;
    return nsk_pattable_convtopng(pattable);
}

/*!
 * \brief  Converts image to PPU colors
 *
 * \param[in]  image   The image
 * \param[in]  x       Component X start position
 * \param[in]  y       Component Y start position
 * \param[out] object  The object
 */
static void _convert_ppucolors_i2o(
    const struct nsk_type_pngimage *image,
    size_t x,
    size_t y,
    void *object
) {
    struct nsk_type_ppucolors *colors = object;
    nsk_ppucolors_convfrompng(image, x, y, colors);
}

/*!
 * \brief  Converts image to palettes
 *
 * \param[in]  image   The image
 * \param[in]  x       Component X start position
 * \param[in]  y       Component Y start position
 * \param[out] object  The object
 */
static void _convert_palettes_i2o(
    const struct nsk_type_pngimage *image,
    size_t x,
    size_t y,
    void *object
) {
    struct nsk_type_palettes *palettes = object;
    nsk_palettes_convfrompng(image, x, y, palettes);
}

/*!
 * \brief  Converts image to pattern table
 *
 * \param[in]  image   The image
 * \param[in]  x       Component X start position
 * \param[in]  y       Component Y start position
 * \param[out] object  The object
 */
static void _convert_pattable_i2o(
    const struct nsk_type_pngimage *image,
    size_t x,
    size_t y,
    void *object
) {
    struct nsk_type_pattable *pattable = object;
    nsk_pattable_convfrompng(image, x, y, pattable);
}

/*!
 * \brief Compose constructing info
 */
static const struct {
    const char *name; /*!< Name of the object               */
    size_t startx;    /*!< Position X in the full image     */
    size_t starty;    /*!< Position Y in the ful image      */
    /*!
     * \brief Object-to-image converter
     */
    struct nsk_type_pngimage *(*conv_o2i)(
        const void *object
    );
    /*!
     * \brief Image-to-object converter
     */
    void (*conv_i2o)(
        const struct nsk_type_pngimage *image,
        size_t x,
        size_t y,
        void *object
    );
} _compose_info[NSK_COMPOSECOMPS_COUNT] = {
    [NSK_COMPOSECOMP_PPUCOLORS] = {
        .name       = "PPU colors",
        .startx     = NSK_COMPOSEPOS_COLORS_X,
        .starty     = NSK_COMPOSEPOS_COLORS_Y,
        .conv_o2i   = _convert_ppucolors_o2i,
        .conv_i2o   = _convert_ppucolors_i2o
    },
    [NSK_COMPOSECOMP_PALETTES] = {
        .name       = "Local palettes",
        .startx     = NSK_COMPOSEPOS_PALETTES_X,
        .starty     = NSK_COMPOSEPOS_PALETTES_Y,
        .conv_o2i   = _convert_palettes_o2i,
        .conv_i2o   = _convert_palettes_i2o
    },
    [NSK_COMPOSECOMP_LEFT] = {
        .name       = "Left pattern table",
        .startx     = NSK_COMPOSEPOS_LEFT_X,
        .starty     = NSK_COMPOSEPOS_LEFT_Y,
        .conv_o2i   = _convert_pattable_o2i,
        .conv_i2o   = _convert_pattable_i2o
    },
    [NSK_COMPOSECOMP_RIGHT] = {
        .name       = "Right pattern table",
        .startx     = NSK_COMPOSEPOS_RIGHT_X,
        .starty     = NSK_COMPOSEPOS_RIGHT_Y,
        .conv_o2i   = _convert_pattable_o2i,
        .conv_i2o   = _convert_pattable_i2o
    },
};

/*!
 * \brief  Creates full Nesokia PNG image from separate components
 *
 * \param[in]  colors    The PPU colors
 * \param[in]  palettes  The local palettes
 * \param[in]  patleft   The left pattern table
 * \param[in]  patright  The pattern tables
 * \return Composite (full) image
 */
struct nsk_type_pngimage *nsk_pngimage_composesave(
    const struct nsk_type_ppucolors *colors,
    const struct nsk_type_palettes  *palettes,
    const struct nsk_type_pattables *pattables
) {
    static const union nsk_type_color4 background = {
        .r = 236,
        .g = 238,
        .b = 236
    };

    const void *objects[] = {
        [NSK_COMPOSECOMP_PPUCOLORS  ] = colors,
        [NSK_COMPOSECOMP_PALETTES   ] = palettes,
        [NSK_COMPOSECOMP_LEFT       ] = nsk_pattables_getbyaddress(
            pattables,
            NSK_PATTABLEADDR_0000
        ),
        [NSK_COMPOSECOMP_RIGHT      ] = nsk_pattables_getbyaddress(
            pattables,
            NSK_PATTABLEADDR_1000
        ),
    };

    static_assert(
        NSK_SIZE(_compose_info) == NSK_SIZE(objects),
        "Error: missing elements in either `_compose_info` or `objects`"
    );

    struct nsk_type_pngimage *image = nsk_pngimage_empty(
        NSK_COMPOSESIZE_FULL_WIDTH,
        NSK_COMPOSESIZE_FULL_HEIGHT
    );

    nsk_pngimage_cellset(
        image,
        0,
        0,
        image->width,
        image->height,
        background
    );

    for (size_t i = 0; i < NSK_SIZE(_compose_info); i++) {
        if (!objects[i]) {
            continue;
        }

        nsk_auto_pifree struct nsk_type_pngimage *component =
            _compose_info[i].conv_o2i(objects[i]);

        nsk_pngimage_combine(
            image,
            component,
            _compose_info[i].startx,
            _compose_info[i].starty
        );
    }

    return image;
}

/*!
 * \brief  Reads the composite Nesokia PNG image
 *
 * \param[in]  filename   The filename
 * \param[out] colors     The colors
 * \param[out] palettes   The palettes
 * \param[out] pattables  The pattern tables
 */
void nsk_pngimage_composeread(
    const char *filename,
    struct nsk_type_ppucolors *colors,
    struct nsk_type_palettes  *palettes,
    struct nsk_type_pattables *pattables
) {
    if (!filename || !colors || !palettes || !pattables) {
        nsk_err(
            "Error: invalid input at %s",
            __PRETTY_FUNCTION__
        );
        abort();
    }

    memset(colors,    0, sizeof(*colors));
    memset(palettes,  0, sizeof(*palettes));
    memset(pattables, 0, sizeof(*pattables));

    enum nsk_pattable_address pataddress[NSK_PLANES_COUNT] = {
        [NSK_PLANE_BACKGROUND] = NSK_PATTABLEADDR_0000,
        [NSK_PLANE_SPRITES   ] = NSK_PATTABLEADDR_1000
    };

    for (size_t p = 0; p < NSK_PLANES_COUNT; p++) {
        nsk_pattable_setplane  (&pattables->plane[p], p);
        nsk_pattable_setaddress(&pattables->plane[p], pataddress[p]);
    }

    void *objects[] = {
        [NSK_COMPOSECOMP_PPUCOLORS  ] = colors,
        [NSK_COMPOSECOMP_PALETTES   ] = palettes,
        [NSK_COMPOSECOMP_LEFT       ] = &pattables->plane[NSK_PLANE_BACKGROUND],
        [NSK_COMPOSECOMP_RIGHT      ] = &pattables->plane[NSK_PLANE_SPRITES]
    };

    static_assert(
        NSK_SIZE(_compose_info) == NSK_SIZE(objects),
        "Error: missing elements in either `_compose_info` or `objects`"
    );

    nsk_auto_pifree struct nsk_type_pngimage *image = nsk_pngimage_read(
        filename
    );

    for (size_t i = 0; i < NSK_COMPOSECOMPS_COUNT; i++) {
        _compose_info[i].conv_i2o(
            image,
            _compose_info[i].startx,
            _compose_info[i].starty,
            objects[i]
        );
    }
}

#endif