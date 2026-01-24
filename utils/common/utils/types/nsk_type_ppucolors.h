#ifndef NSK_TYPE_PPUCOLORS
#define NSK_TYPE_PPUCOLORS

#if defined(NSK_MODULE_COMPOSE)

#include <stdbool.h>
#include <stddef.h>

#include "../types/nsk_type_color4.h"

/*!
 * \brief Common constants
 */
enum {
    NSK_PPUCOLOR_INVALID = 0xffff /*!< Bad index indicator */
};

/*!
 * \file PPU colors type
 *
 * Provides the routines to work with PPU colors list:
 *
 * ~~~
 *        format         │         reading          │        writing
 * ━━━━━━━━━━━━━━━━━━━━━━┿━━━━━━━━━━━━━━━━━━━━━━━━━━┿━━━━━━━━━━━━━━━━━━━━━━━
 *  NES emulator palette │ nsk_ppucolors_readpal    │ nsk_ppucolors_savepal
 *  GIMP palette         │                          │ nsk_ppucolors_savegpl
 *  Nesokia Colors PNG   │ nsk_ppucolors_readpng    │ nsk_ppucolors_savepng
 * ~~~
 *
 * \note PNG related functions are separated to PNG submodule and require
 * `NSK_MODULE_PNG` to be set
 */

/*!
 * \brief  PPU colors typical sizes
 */
enum nsk_ppucolors_table {
    NSK_PPUCOLORSTABLE_WIDTH  = 16, /*!< Width of the colors table  */
    NSK_PPUCOLORSTABLE_HEIGHT = 4,  /*!< Height of the colors table */

    NSK_PPUCOLORSTABLE_COUNT  =     /*!< Total number of colors     */
        NSK_PPUCOLORSTABLE_WIDTH * NSK_PPUCOLORSTABLE_HEIGHT
};

/*!
 * \brief  PPU colors type
 *
 * \note The entities are grouped by type instead by record to simplify the
 * reading and writing
 */
struct nsk_type_ppucolors {
    union nsk_type_color4 colors[NSK_PPUCOLORSTABLE_COUNT]; /*!< List of colors */
    bool                 allowed[NSK_PPUCOLORSTABLE_COUNT]; /*!< Availability   */
};

/*!
 * \brief  Reads the PPU colors from the .pal file
 *
 * \param[in] filename  The filename
 * \return PPU colors
 */
struct nsk_type_ppucolors nsk_ppucolors_readpal(
    const char *filename
);

/*!
 * \brief  Saves the PPU colors as .pal file
 *
 * \param[in] filename  The filename
 * \param[in] colors    The colors
 */
void nsk_ppucolors_savepal(
    const char *filename,
    const struct nsk_type_ppucolors *colors
);

/*!
 * \brief  Saves the PPU colors as .gpl (GIMP palette) file
 *
 * \param[in] filename  The filename
 * \param[in] colors    The colors */
void nsk_ppucolors_savegpl(
    const char *filename,
    const struct nsk_type_ppucolors *colors
);

/*!
 * \brief  Shows the PPU colors as ANSI colored output
 *
 * \param[in] colors  The colors
 */
void nsk_ppucolors_show(
    const struct nsk_type_ppucolors *colors
);

/*!
 * \brief  Lookups the index of the color in the PPU colors list
 *
 * \param[in] colos PPU colors list
 * \param[in] color  The color
 * \return Index or NSK_PPUCOLOR_INVALID
 */
size_t nsk_ppucolors_lookup(
    const struct nsk_type_ppucolors *colors,
    const union nsk_type_color4 *color
);

/*!
 * \brief  Validates the PPU colors list
 *
 * \param[in] colors  The colors
 */
void nsk_ppucolors_validate(
    const struct nsk_type_ppucolors *colors
);

#endif

#endif