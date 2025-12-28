#ifndef NSK_TYPE_NAMETABLE
#define NSK_TYPE_NAMETABLE

#include "../types/nsk_type_planes.h"
#include "../types/nsk_type_tile.h"

/*!
 * \brief  Nametables PPU address variations
 */
enum nsk_nametable_address {
    NSK_NAMETABLEADDR_0000, /*!< Left table  */
    NSK_NAMETABLEADDR_1000, /*!< Right table */

    NSK_NAMETABLEADDRS_COUNT /*!< Number of nametables per image */
};

/*!
 * \brief  Nametables positioning
 */
enum nsk_nametable_pos {
    NSK_NAMETABLEPOS_LEFT_X     = 24,   /*!< Left table data start X pos    */
    NSK_NAMETABLEPOS_LEFT_Y     = 32,   /*!< Left table data start Y pos    */
    NSK_NAMETABLEPOS_RIGHT_X    = 184,  /*!< Right table data start X pos   */
    NSK_NAMETABLEPOS_RIGHT_Y    = 32    /*!< Right table data start Y pos   */
};

/*!
 * \brief  Nametable sizes configuration
 */
enum nsk_nametable_size {
    NSK_NAMETABLE_WIDTH  = 16, /*!< Width in tiles  */
    NSK_NAMETABLE_HEIGHT = 16, /*!< Height in tiles */
};

/*!
 * \brief  Nametables's cell sizes
 */
enum nsk_nametable_cell {
    NSK_NAMETABLECELL_WIDTH  = 8, /*!< Width of the cell in pixels  */
    NSK_NAMETABLECELL_HEIGHT = 8, /*!< Height of the cell in pixels */
};

/*!
 * \brief  Single nametable
 */
struct nsk_type_nametable {
    enum nsk_nametable_address address;  /*!< Nametable's address */
    struct nsk_type_tile tile            /*!< Tiles data          */
        [NSK_NAMETABLE_HEIGHT]
        [NSK_NAMETABLE_WIDTH];
};

/*!
 * \brief  Set of nametables
 */
struct nsk_type_nametables {
    struct nsk_type_nametable plane[NSK_PLANES_COUNT];
};

/*!
 * \brief  Converts the enum value to the raw value
 *
 * \param[in] address  The address
 * \return Raw address value
 */
unsigned nsk_conv_address2value(enum nsk_nametable_address address);

/*!
 * \brief  Reads the nametables from the input image
 */
void nsk_nametables_read(void);

/*!
 * \brief  Validates the nametables
 */
void nsk_nametables_validate(void);

#endif