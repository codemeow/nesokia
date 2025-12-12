#ifndef NSK_HEADER_TABLE
#define NSK_HEADER_TABLE

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../../types/header/nsk_header_category.h"
#include "../../types/header/nsk_header_data.h"
#include "../../types/header/nsk_header_raw.h"
#include "../../types/pair/nsk_pair_cmp.h"

/*!
 * "Not applicable" value
 */
#define NSK_TABLE_NAN "-"

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
     * Printing length for printf-like printers
     */
    size_t length;

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
     * Value converter to string. Fits JSON rules and any similar formats
     * like YAML,HCL etc
     */
    const char *(*conv_json)(const void *field);

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
        enum nsk_pair_operator operator
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

#endif