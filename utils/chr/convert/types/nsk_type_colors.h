#ifndef NSK_TYPE_COLORS
#define NSK_TYPE_COLORS

#include <stdbool.h>

#include "../types/nsk_type_color.h"

/*!
 * \brief  Positioning of the global palette in the image
 */
enum nsk_colors_pos {
    NSK_COLORSPOS_X = 24,  /*!< X position data start */
    NSK_COLORSPOS_Y = 200, /*!< Y position data start */
};

/*!
 * \brief  The sizes of the global palette
 */
enum nsk_colors_size {
    NSK_COLORS_WIDTH  = 16, /*!< Width of the colors table  */
    NSK_COLORS_HEIGHT = 4,  /*!< Height of the colors table */

    NSK_COLORS_COUNT =      /*!< Total number of colors     */
        NSK_COLORS_WIDTH * NSK_COLORS_HEIGHT,
};

/*!
 * \brief  The colors cells sizes
 */
enum nsk_colors_cell {
    NSK_COLORSCELL_WIDTH  = 8, /*!< Width of the colors cells in pixels  */
    NSK_COLORSCELL_HEIGHT = 8, /*!< Height of the colors cells in pixels */
};

/*!
 * \brief  List of the global palette colors
 *
 * \note The allowed colors are the list of the colors that could be safely used
 * over the picture. The most well known example is the lack of $0d color in the
 * standard NTSC palette.
 * To mark the color as not allowed make the color cell multicolored, draw a
 * character, a cross, or any sign of your choosing.
 */
struct nsk_type_colors {
    struct nsk_type_color colors[NSK_COLORS_COUNT]; /*!< List of colors          */
    bool                 allowed[NSK_COLORS_COUNT]; /*!< List of allowed colors  */
};

/*!
 * \brief  Reads the global palette from the input file
 */
void nsk_colors_read(void);

/*!
 * \brief  Validates the global palette
 */
void nsk_colors_validate(void);

/*!
 * \brief  Lookups the index of the color in the global palette
 *
 * \param[in] color  The color
 * \return Index or NSK_COLOR_INVALID
 */
unsigned nsk_colors_index(const struct nsk_type_color *color);

#endif