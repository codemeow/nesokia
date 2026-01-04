#ifndef NSK_ARGS_OPTIONS
#define NSK_ARGS_OPTIONS

#include <stdlib.h>
#include <stdbool.h>

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
 * \brief  Provided program options
 */
struct nsk_options_program {
    struct {
        const char  *colors;               /*!< Input .pal file               */
        const char  *palback;              /*!< Background palette input      */
        const char  *palsprites;           /*!< Sprites palette input         */
        const char  *palmerged;            /*!< Merged palette input          */
    } input;
    struct {
        const char  *colors;               /*!< Output PPU PNG file name      */
        const char  *palettes;             /*!< Output palettes PNG file name */
        bool         ansi;                 /*!< ANSI terminal output          */
    } output;

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