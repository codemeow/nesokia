#ifndef NSK_TYPE_PALETTES
#define NSK_TYPE_PALETTES

#if defined(NSK_MODULE_COMPOSE)

/*!
 * \file PPU colors type
 *
 * Provides the routines to work with PPU colors list:
 *
 * ~~~
 *         format          │         reading         │        writing
 * ━━━━━━━━━━━━━━━━━━━━━━━━┿━━━━━━━━━━━━━━━━━━━━━━━━━┿━━━━━━━━━━━━━━━━━━━━━━━
 *  Local palette ($3f00)  │ nsk_palettes_readspal   │ nsk_palettes_savespal
 *  Local palettes ($3f00) │ nsk_palettes_readspals  │ nsk_palettes_savespals
 *  Nesokia Colors PNG     │ nsk_palettes_readpng    │ nsk_palettes_savepng
 * ~~~
 *
 * \note PNG related functions are separated to PNG submodule and require
 * `NSK_MODULE_PNG` to be set
 */

#include <stddef.h>

#include "../types/nsk_type_plane.h"
#include "../types/nsk_type_color4.h"
#include "../types/nsk_type_ppucolors.h"

/*!
 * \brief  Palettes sizes
 */
enum nsk_palette_size {
    NSK_PALETTESIZE_GROUPS = 4, /*!< Number of palette groups       */
    NSK_PALETTESIZE_COLORS = 4, /*!< Number of colors per group     */
};

/*!
 * \brief  Single palette type
 */
struct nsk_type_palette {
    /*!
     * \brief Palette groups
     */
    struct {
        /*!
         * \brief Color
         */
        union nsk_type_color4 color[NSK_PALETTESIZE_COLORS];
        /*!
         * Index in the global palette
         */
        size_t                index[NSK_PALETTESIZE_COLORS];
    } group[NSK_PALETTESIZE_GROUPS];

    /*!
     * \brief  Duplicating value of the selected plane to
     * explicitly state the plane in case of separate processing
     */
    enum nsk_plane_list plane;

    /*!
     * \brief Initialization flags
     */
    struct {
        bool colors;  /*!< Colors are initialized  */
        bool indexes; /*!< Indexes are initialized */
    } init;
};

/*!
 * \brief  Full palettes list
 */
struct nsk_type_palettes {
    /*!
     * \brief Palettes by planes
     */
    struct nsk_type_palette plane[NSK_PLANES_COUNT];
};

/*!
 * \brief  Validates palette's colors field
 *
 * \param[in]  table  The table
 */
void nsk_palette_validate_colors(const struct nsk_type_palette *palette);

/*!
 * \brief  Validates palette's indexes field
 *
 * \param[in]  table  The table
 */
void nsk_palette_validate_indexes(const struct nsk_type_palette *palette);

/*!
 * \brief  Reads .spal file ("Selected palette")
 * (binary $3f00..$3f0f/$f10..$3f1f)
 *
 * \note Reads only `index` fiels. Call `nsk_palette_apply` function
 *       to fill the actual colors
 *
 * \param[in] filename  The filename
 * \return Palette
 */
struct nsk_type_palette nsk_palette_readspal(
    const char *filename
);

/*!
 * \brief  Saves selected palette as binary .spal file
 *
 * \param[in] filename  The filename
 * \param[in] palette   The palette
 */
void nsk_palette_savespal(
    const char *filename,
    const struct nsk_type_palette *palette
);

/*!
 * \brief  Reads .spals file ("Selected palettes")
 * (binary $3f00..$3f1f)
 *
 * \note Reads only `index` fiels. Call `nsk_palette_apply` function
 *       to fill the actual colors
 *
 * \param[in] filename  The filename
 * \return Palettes
 */
struct nsk_type_palettes nsk_palettes_readspals(
    const char *filename
);

/*!
 * \brief  Saves selected palettes as binary .spals file
 *
 * \param[in] filename  The filename
 * \param[in] palette   The palette
 */
void nsk_palette_savespals(
    const char *filename,
    const struct nsk_type_palettes *palettes
);

/*!
 * \brief  Shows selected palette as ANSI colored output
 *
 * \param[in] palette  The palette
 */
void nsk_palette_show(
    const struct nsk_type_palette *palette
);

/*!
 * \brief  Shows selected palettes as ANSI colored output
 *
 * \param[in] palettes  The palettes
 */
void nsk_palettes_show(
    const struct nsk_type_palettes *palettes
);

/*!
 * \brief  Validates the single palette
 *
 * \param[in] colors   The colors
 * \param[in] palette  The palette
 */
void nsk_palette_validate(
    const struct nsk_type_ppucolors *colors,
    const struct nsk_type_palette   *palette
);

/*!
 * \brief  Validates all palettes
 *
 * \param[in] colors   The colors
 * \param[in] palettes  The palettes
 */
void nsk_palettes_validate(
    const struct nsk_type_ppucolors *colors,
    const struct nsk_type_palettes  *palettes
);

/*!
 * \brief  Applies the PPU colors to the palette using `index` field
 *
 * \param[in]       colors   The colors
 * \param[in,out]   palette  The palette
 */
void nsk_palette_setcolors(
    const struct nsk_type_ppucolors *colors,
    struct nsk_type_palette *palette
);

/*!
 * \brief  Applies the PPU colors to the palettes using `index` field
 *
 * \param[in]       colors    The colors
 * \param[in,out]   palettes  The palettes
 */
void nsk_palettes_setcolors(
    const struct nsk_type_ppucolors *colors,
    struct nsk_type_palettes *palettes
);

/*!
 * \brief  Finds the color index in the selected palette
 *
 * \param[in] palette  The palette
 * \param[in] group The colors group
 * \return The color index
 */
size_t nsk_palette_getindex(
    const struct nsk_type_palette *palette,
    size_t group,
    const union nsk_type_color4 *color
);

#endif

#endif