#ifndef NSK_HEADER_TABLE
#define NSK_HEADER_TABLE

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../ROM/header/nsk_header_raw.h"
#include "../../ROM/header/nsk_header_data.h"

/*!
 * \brief  Match function possible oparators
 */
enum nsk_match_operator {
    NSK_MATCH_EQUAL,        /*!< =, equal               */
    NSK_MATCH_NOTEQUAL,     /*!< !=, not equal          */
    NSK_MATCH_LESS,         /*!< <, less than           */
    NSK_MATCH_LESSOREQUAL,  /*!< <=, less or equal      */
    NSK_MATCH_MORE,         /*!< >, more than           */
    NSK_MATCH_MOREOREQUAL   /*!< >=, more or equal      */
};

/*!
 * \brief  Row categories for grouping
 */
enum nsk_table_category {
    NSK_CATEGORY_NONE,          /*!< Initial state              */
    NSK_CATEGORY_FORMAT,        /*!< iNES/NES2.2048             */
    NSK_CATEGORY_CONSOLE,       /*!< Console type/region/etc    */
    NSK_CATEGORY_NAMETABLES,    /*!< Nametables info            */
    NSK_CATEGORY_MAPPER,        /*!< Mapper info                */
    NSK_CATEGORY_MEMORY,        /*!< Memory info (RAM/ROM/NV)   */
    NSK_CATEGORY_MISC           /*!< Miscellaneous info         */
};

/*!
 * \brief  Global header entry structure
 */
struct nsk_header_tableentry {
    /*!
     * \brief  Row category
     */
    enum nsk_table_category category;

    /*!
     * Detailed desciption
     */
    const char *description;

    /*!
     * User-used named shortcut, used for filtering, searching, etc.
     */
    const char *shortcut;

    /*!
     * The associated field shift from the beginning of the #nsk_header_data
     * struct's memory block. Used to match the tpassed field in the
     * processors
     */
    ptrdiff_t shift;

    /*!
     * Converts the raw header values to the structured #nsk_header_data
     */
    void (*conv_raw)(
        const struct nsk_header_raw *raw,
        struct nsk_header_data *data
    );

    /*!
     * Value converter to string. Uses expanded long form to convert, i.e.
     * enum description or converted bytes
     */
    const char *(*conv_long)(const void *field);

    /*!
     * Value converter to string. Uses short form to convert, i.e. integer
     * value instead of human-readable bool
     */
    const char *(*conv_short)(const void *field);

    /*!
     * Check if the provided value somehow matches the field value with the
     * provided operator, i.e.
     * `field = 5, value = 4, operator = NSK_MATCH_MOREOREQUAL` - true
     * (i.e. `mapperid>=304` and the field (mapperid) was 305)
     * `field = 3, value = 3, operator = NSK_MATCH_NOTEQUAL` - false
     * (i.e. `prgrom!=2048` and the field (prgrom) was exactly 2048)
     */
    bool (*match)(
        const void *field,
        uint64_t value,
        enum nsk_match_operator operator
    );
};

/*!
 * The global header data table for the processors syncing
 */
extern const struct nsk_header_tableentry nsk_header_table[];

/*!
 * The size of the global header table;
 */
extern const size_t nsk_header_tablesize;

/*!
 * \brief  Returns the fancy name of the category
 *
 * \param[in] category  The category
 * \return Static string
 */
const char *nsk_category_name(enum nsk_table_category category);

#endif