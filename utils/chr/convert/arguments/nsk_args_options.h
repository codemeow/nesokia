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
    NSK_PPUCTRL34_NORMAL, /*!< Left is background  */
    NSK_PPUCTRL34_INVERT, /*!< Right is background */

    NSK_PPUCTRL34S_COUNT, /*!< Number of elements  */
};

/*!
 * \brief  Provided program options
 */
struct nsk_options_program {
    struct {
        const char *output;                /*!< Global output directory       */
    } directory;

    struct {
        const char *nametable[NSK_PLANES_COUNT]; /*!< Nametables output names */
        const char *palettes [NSK_PLANES_COUNT]; /*!< Palettes output names   */
    } output;

    struct {
        const char *template;              /*!< Template input name           */
        const char *left;                  /*!< Left nametable input name     */
        const char *right;                 /*!< Right nametable input name    */
        const char *colors;                /*!< Global colors input name      */
        const char *palettes;              /*!< Local colors input name       */
    } input;

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