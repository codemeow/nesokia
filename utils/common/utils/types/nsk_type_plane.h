#ifndef NSK_TYPE_PLANE
#define NSK_TYPE_PLANE

/*!
 * \brief  Types of planes for palettes, nametables, etc
 */
enum nsk_plane_list {
    NSK_PLANE_BACKGROUND, /*!< Background layer */
    NSK_PLANE_SPRITES,    /*!< Sprites layer    */

    NSK_PLANES_COUNT      /*!< Number of layers */
};

/*!
 * \brief  Converts the plane enum into string description
 *
 * \param[in]  value  The value
 * \return Static string
 */
const char *nsk_conv_plane2string(enum nsk_plane_list value);

#endif