#include <stdlib.h>

#include "../../types/header/nsk_header_category.h"

#include "../../utils/log/nsk_log_err.h"

/*!
 * \brief  Returns the fancy name of the category
 *
 * \param[in] category  The category
 * \return Static string
 */
const char *nsk_category_name(enum nsk_table_category category) {
    switch (category) {
        case NSK_CATEGORY_NONE:
            nsk_err("Error: \"NONE\" category should not be printed\n");
            abort();

        case NSK_CATEGORY_FORMAT    : return "Format";
        case NSK_CATEGORY_CONSOLE   : return "Console";
        case NSK_CATEGORY_NAMETABLES: return "Nametables";
        case NSK_CATEGORY_MAPPER    : return "Mapper";
        case NSK_CATEGORY_MEMORY    : return "Memory";
        case NSK_CATEGORY_MISC      : return "Miscellaneous info";
    }

    nsk_err("Error: unknown category: %d\n", (int)category);
    abort();
}

/*!
 * \brief  Return the JSON-valid short name of the category
 *
 * \param[in] category  The category
 * \return Static string
 */
const char *nsk_category_json(enum nsk_table_category category) {
    switch (category) {
        case NSK_CATEGORY_NONE:
            nsk_err("Error: \"NONE\" category should not be printed\n");
            abort();

        case NSK_CATEGORY_FORMAT    : return "format";
        case NSK_CATEGORY_CONSOLE   : return "console";
        case NSK_CATEGORY_NAMETABLES: return "nametables";
        case NSK_CATEGORY_MAPPER    : return "mapper";
        case NSK_CATEGORY_MEMORY    : return "memory";
        case NSK_CATEGORY_MISC      : return "misc";
    }

    nsk_err("Error: unknown category: %d\n", (int)category);
    abort();
}