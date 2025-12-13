#ifndef NSK_TYPE_TEMPLATE
#define NSK_TYPE_TEMPLATE

/*!
 * \brief  Sizes of the full input image
 */
enum nsk_size_full {
    NSK_FULL_WIDTH  = 328, /*!< Width  */
    NSK_FULL_HEIGHT = 248  /*!< Height */
};

/*!
 * \brief  Data start positions of the components
 */
enum nsk_position_components {
    NSK_POS_LEFT_X     = 24,  /*!< Left table data start X      */
    NSK_POS_LEFT_Y     = 32,  /*!< Left table data start Y      */

    NSK_POS_RIGHT_X    = 184, /*!< Right table data start X     */
    NSK_POS_RIGHT_Y    = 32,  /*!< Right table data start Y     */

    NSK_POS_COLORS_X   = 24,  /*!< Global colors data start X   */
    NSK_POS_COLORS_Y   = 200, /*!< Global colors data start Y   */

    NSK_POS_PALETTES_X = 184, /*!< Palettes data start X        */
    NSK_POS_PALETTES_Y = 200  /*!< Palettes data start Y        */
};

/*!
 * \brief  Data sizes for the components
 */
enum nsk_size_components {
    NSK_COMPONENT_TILE_WIDTH     = 8,  /*!< Width in pixels         */
    NSK_COMPONENT_TILE_HEIGHT    = 8,  /*!< Height in pixels        */

    NSK_COMPONENT_LEFT_WIDTH     = 16, /*!< Width in tiles          */
    NSK_COMPONENT_LEFT_HEIGHT    = 16, /*!< Height in tiles         */

    NSK_COMPONENT_RIGHT_WIDTH    = 16, /*!< Width in tiles          */
    NSK_COMPONENT_RIGHT_HEIGHT   = 16, /*!< Height in tiles         */

    NSK_COMPONENT_COLORS_WIDTH   = 16, /*!< Width in tiles          */
    NSK_COMPONENT_COLORS_HEIGHT  = 4,  /*!< Height in tiles         */

    NSK_COMPONENT_PALETTE_WIDTH  = 16, /*!< Width in tiles          */
    NSK_COMPONENT_PALETTE_HEIGHT = 2,  /*!< Height in tiles         */
    NSK_COMPONENT_PALETTE_COUNT  = 4,  /*!< Horizontal count        */
    NSK_COMPONENT_PALETTE_PICK_X = 1,  /*!< Horizontal picker shift */
    NSK_COMPONENT_PALETTE_PICK_Y = 1,  /*!< Vertical picker shift   */
};

/*!
 * \brief  Sizes of the base template composite component
 */
enum nsk_size_template {
    NSK_TEMPLATE_WIDTH      = NSK_FULL_WIDTH,   /*!< Width  */
    NSK_TEMPLATE_HEIGHT     = NSK_FULL_HEIGHT,  /*!< Height */

    NSK_TEMPLATE_LEFT_X     = 8,                /*!< X position in template */
    NSK_TEMPLATE_LEFT_Y     = 8,                /*!< Y position in template */
    NSK_TEMPLATE_RIGHT_X    = 168,              /*!< X position in template */
    NSK_TEMPLATE_RIGHT_Y    = 8,                /*!< Y position in template */
    NSK_TEMPLATE_COLORS_X   = 8,                /*!< X position in template */
    NSK_TEMPLATE_COLORS_Y   = 176,              /*!< Y position in template */
    NSK_TEMPLATE_PALETTE_X  = 168,              /*!< X position in template */
    NSK_TEMPLATE_PALETTE_Y  = 176,              /*!< Y position in template */
};

/*!
 * \brief Sizes of the left table composite component
 */
enum nsk_size_left {
    NSK_LEFT_WIDTH     = 152, /*!< Width  */
    NSK_LEFT_HEIGHT    = 160  /*!< Height */
};

/*!
 * \brief Sizes of the right table composite component
 */
enum nsk_size_right {
    NSK_RIGHT_WIDTH     = 152, /*!< Width  */
    NSK_RIGHT_HEIGHT    = 160  /*!< Height */
};

/*!
 * \brief Sizes of the colors composite component
 */
enum nsk_size_colors {
    NSK_COLORS_WIDTH     = 152, /*!< Width  */
    NSK_COLORS_HEIGHT    = 64   /*!< Height */
};

/*!
 * \brief Sizes of the palettes composite component
 */
enum nsk_size_palettes {
    NSK_PALETTES_WIDTH     = 152, /*!< Width  */
    NSK_PALETTES_HEIGHT    = 64   /*!< Height */
};

#endif