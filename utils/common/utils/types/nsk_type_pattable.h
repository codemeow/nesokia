#ifndef NSK_TYPE_PATTABLE
#define NSK_TYPE_PATTABLE

#if defined(NSK_MODULE_COMPOSE)

#include <stdbool.h>

#include "../types/nsk_type_plane.h"
#include "../types/nsk_type_tile.h"
#include "../types/nsk_type_palettes.h"

/*!
 * \brief  Pattern tables addresses
 */
enum nsk_pattable_address {
    NSK_PATTABLEADDR_0000, /*!< Left table  */
    NSK_PATTABLEADDR_1000, /*!< Right table */
};

/*!
 * \brief  Pattern tables sizes configuration
 */
enum nsk_pattable_table {
    NSK_PATTABLETABLE_WIDTH  = 16, /*!< Width in tiles  */
    NSK_PATTABLETABLE_HEIGHT = 16, /*!< Height in tiles */
};

/*!
 * \brief  Single pattern table
 */
struct nsk_type_pattable {
    /*!
     * \brief  Pattern table plane index
     *
     * Usually left table ($0000) is background, right ($1000) is sprites,
     * regulated by PPUCTRL bits 3 and 4
     *
     */
    enum nsk_plane_list plane;
    /*!
     * \brief  Table address ($0000/$1000)
     */
    enum nsk_pattable_address address;
    /*!
     * \brief  Pattern table tiles
     */
    struct nsk_type_tile tile[NSK_PATTABLETABLE_HEIGHT][NSK_PATTABLETABLE_WIDTH];

    /*!
     * \brief  Initialization flags
     *
     * Used for detecting wrong workflow steps order
     */
    struct {
        bool plane;         /*!< Plane value is initialized     */
        bool address;       /*!< Address value is initialized   */
    } init;
};

/*!
 * \brief  Set of pattern tables
 */
struct nsk_type_pattables {
    struct nsk_type_pattable plane[NSK_PLANES_COUNT];
};

/*!
 * \brief  Validates pattern table's plane field
 *
 * \param[in]  table  The table
 */
void nsk_pattable_validate_plane(const struct nsk_type_pattable *table);

/*!
 * \brief  Validates pattern table's address field
 *
 * \param[in]  table  The table
 */
void nsk_pattable_validate_address(const struct nsk_type_pattable *table);

/*!
 * \brief  Converts the enum value to the raw value
 *
 * \param[in] address  The address
 * \return Raw address value
 */
unsigned nsk_conv_pataddress2value(enum nsk_pattable_address address);

/*!
 * \brief  Reads pattern table from .pat file
 * (binary $0000..$0fff/$1000..$1fff)
 *
 * \param[in] filename  The filename
 * \return The resulting pattern table
 */
struct nsk_type_pattable nsk_pattable_readpat(
    const char *filename
);

/*!
 * \brief  Reads pattern tables from .pats file
 * (binary $0000..$1fff)
 *
 * \param[in] filename  The filename
 * \return The resulting pattern tables
 */
struct nsk_type_pattables nsk_pattables_readpats(
    const char *filename
);

/*!
 * \brief  Saves selected pattern table as .pat file
 *
 * \param[in] filename  The filename
 * \param[in] table     The table
 */
void nsk_pattable_savepat(
    const char *filename,
    const struct nsk_type_pattable *table
);

/*!
 * \brief  Saves pattern tables as .pats file
 *
 * \note The palette indexes should be applied first with
 * nsk_pattable_apply
 *
 * \param[in] filename  The filename
 * \param[in] tables    The tables
 */
void nsk_pattables_savepats(
    const char *filename,
    const struct nsk_type_pattables *tables
);

/*!
 * \brief  Shows selected pattern table as ANSI colored output
 *
 * \param[in] table  The pattern table
 */
void nsk_pattable_show(
    const struct nsk_type_pattable *table
);

/*!
 * \brief  Shows pattern tables as ANSI colored output
 *
 * \param[in] tables  The pattern tables
 */
void nsk_pattables_show(
    const struct nsk_type_pattables *tables
);

/*!
 * \brief  Assigns the tiles properties to the selected pattern table
 *
 * \param[in] palette  The palette
 * \param[in] table    The table
 */
void nsk_pattable_settiles(
    const struct nsk_type_palette  *palette,
    struct nsk_type_pattable *table
);

/*!
 * \brief  Assigns the tiles properties to the patter tables
 *
 * \param[in] palettes  The palettes
 * \param[in] tables    The tables
 */
void nsk_pattables_settiles(
    const struct nsk_type_palettes  *palettes,
    struct nsk_type_pattables *tables
);

/*!
 * \brief  By address accessor
 *
 * \param[in] tables   The tables
 * \param[in] address  The address
 * \return Matching pattern table
 */
const struct nsk_type_pattable *nsk_pattables_getbyaddress(
    const struct nsk_type_pattables *tables,
    enum nsk_pattable_address address
);

/*!
 * \brief  Sets the plane field and marks the init section
 *
 * \param[in,out]  table  The table
 * \param[in]      plane  The plane value
 */
void nsk_pattable_setplane(
    struct nsk_type_pattable *table,
    enum nsk_plane_list plane
);

/*!
 * \brief  Sets the address field and marks the init section
 *
 * \param[in,out]  table   The table
 * \param[in]      address The address value
 */
void nsk_pattable_setaddress(
    struct nsk_type_pattable *table,
    enum nsk_pattable_address address
);

#endif

#endif