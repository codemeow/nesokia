#ifndef NSK_TYPE_PALETTE
#define NSK_TYPE_PALETTE

#include "../types/nsk_type_color.h"
#include "../types/nsk_type_planes.h"

/*!
 * \brief  Positioning of the local palette in the image
 */
enum nsk_palette_pos {
    NSK_PALETTEPOS_BACK_X   = 184, /*!< X position back data start      */
    NSK_PALETTEPOS_BACK_Y   = 200, /*!< Y position back data start      */
    NSK_PALETTEPOS_SPRITE_X = 184, /*!< X position sprite data start    */
    NSK_PALETTEPOS_SPRITE_Y = 208, /*!< Y position sprite data start    */
};

/*!
 * \brief  Palettes sizes
 */
enum nsk_palette_size {
    NSK_PALETTE_GROUPS = 4, /*!< Number of palette groups       */
    NSK_PALETTE_COLORS = 4, /*!< Number of colors per group     */
};

/*!
 * \brief  The palettes cells sizes
 */
enum nsk_palette_cell {
    NSK_PALETTECELL_WIDTH  = 8, /*!< The width of the palette cell in pixels  */
    NSK_PALETTECELL_HEIGHT = 8, /*!< The height of the palette cell in pixels */

    NSK_PALETTECELL_PADX   = 1, /*!< The Left padding in pixels */
    NSK_PALETTECELL_PADY   = 1, /*!< The Top padding in pixels  */

    NSK_PALETTECELL_COLS   = 16,/*!< Number of cols in tiles    */
    NSK_PALETTECELL_ROWS   = 2, /*!< Number of rows in tiles    */
};

/*!
 * \brief  Single plane palette
 */
struct nsk_type_palette {
    /*
     * Note:
     *     While in the tiles the `palette.color` index serves as
     *     the index of the local palette color the same field in
     *     the palette itself points to the index of the global
     *     palette (called `colors` in this project for clarity)
     */

    struct {
        struct nsk_type_color color[NSK_PALETTE_COLORS]; /*!< Colors */
    } group[NSK_PALETTE_GROUPS];                    /*!< Groups */
};

/*!
 * \brief  Full palettes
 */
struct nsk_type_palettes {
    struct nsk_type_palette planes[NSK_PLANES_COUNT];    /*!< Planes */
};

/*!
 * \brief  Reads the local palettes from the input file
 */
void nsk_palettes_read(void);

/*!
 * \brief  Validates the local palettes
 */
void nsk_palettes_validate(void);

#endif