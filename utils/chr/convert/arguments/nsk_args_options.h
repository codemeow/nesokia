#ifndef NSK_ARGS_OPTIONS
#define NSK_ARGS_OPTIONS

#include <stdlib.h>
#include <stdbool.h>

#include "../types/nsk_type_planes.h"

/*!
 * \brief  The starting point of the short-option replacing value
 *
 * Use this value when the entry lacks of the short option. Increment the value
 * for each entry of this kind
 */
#define NSK_OPTION_SHORTLIMIT (1000)

/*!
 * \brief  Options table entry
 */
struct nsk_options_entry {
    const char   *option_long;             /*!< Long argument option or NULL  */
    const int     option_short;            /*!< Short argument option or 1000+*/
    const int     option_arg;              /*!< Argument presence flag        */
    void        (*option_processor)(void); /*!< Argument processor            */
    const char   *option_desc;             /*!< Argument description          */
};

/*!
 * \brief  PPUCTRL 3th and 4th bits values
 */
enum nsk_ppuctlr34_values {
    NSK_PPUCTRL34_BACK0000, /*!< Left is background  */
    NSK_PPUCTRL34_BACK1000, /*!< Right is background */

    NSK_PPUCTRL34S_COUNT, /*!< Number of elements  */
};

enum nsk_mode_values {
    NSK_MODE_TEMPLATE2CHR, /*!< PNG template to CHR conversion */
    NSK_MODE_CHR2TEMPLATE, /*!< CHR to PNG template conversion */

    NSK_MODES_COUNT, /*!< Number of modes */
}

/*!
 * \brief  Provided program options
 */
struct nsk_options_program {
    struct {
        const char *full;           /*!< Full PNG template (PNG only)   */
        const char *ppucolors;      /*!< PPU colors (PNG/CHR)           */
        struct {
            const char *both;       /*!< Palettes (PNG/CHR)             */
            const char *back;       /*!< Background palette (CHR only)  */
            const char *sprites;    /*!< Sprites palette (CHR only)     */
        } palettes;
        struct {
            const char *both;       /*!< Both pattern tables (CHR only) */
            const char *left;       /*!< Left pattern table (PNG/CHR)   */
            const char *right;      /*!< Right pattern table (PNG/CHR)  */
        } pattables;
    } input;

    struct {
        const char *full;           /*!< Full PNG template (PNG only)   */
        const char *ppucolors;      /*!< PPU colors (PNG/CHR)           */
        struct {
            const char *both;       /*!< Palettes (PNG/CHR)             */
            const char *back;       /*!< Background palette (CHR only)  */
            const char *sprites;    /*!< Sprites palette (CHR only)     */
        } palettes;
        struct {
            const char *both;       /*!< Both pattern tables (CHR only) */
            const char *left;       /*!< Left pattern table (PNG/CHR)   */
            const char *right;      /*!< Right pattern table (PNG/CHR)  */
        } pattables;
    } output;

    enum nsk_mode_values      mode;        /*!< Selected mode                 */
    enum nsk_ppuctlr34_values ppuctrl34;   /*!< Nametables purpose            */

    char *const     *files;                /*!< Null-terminated files list    */
};

/*!
 * Available options table
 */
extern struct nsk_options_entry nsk_options_table[];

/*!
 * Available options table size
 */
extern size_t nsk_options_count;

/*!
 * \brief  Provided program options
 */
extern struct nsk_options_program nsk_options_program;

#endif