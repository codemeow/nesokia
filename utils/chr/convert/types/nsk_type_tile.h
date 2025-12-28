#ifndef NSK_TYPE_TILE
#define NSK_TYPE_TILE

#include "../types/nsk_type_color.h"

/*!
 * \brief  Tile sizes configuration
 */
enum nsk_tile_size {
    NSK_TILE_WIDTH  = 8, /*!< Width in pixels  */
    NSK_TILE_HEIGHT = 8, /*!< Height in pixels */
};

/*!
 * \brief  Single tile
 */
struct nsk_type_tile {
    unsigned palette;               /*!< Tile palette (0..3) */
    struct nsk_type_color pixel          /*!< Tile pixels data    */
        [NSK_TILE_HEIGHT]
        [NSK_TILE_WIDTH];
};

#endif