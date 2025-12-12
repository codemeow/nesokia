#include <stddef.h>

#include "../../../types/header/enums/nsk_enum_console.h"

#include "../../../utils/nsk_util_multimap.h"

/*!
 * \brief  Returns string description of the console type
 *
 * \param[in] type  The type
 * \return  Static string
 */
const char *nsk_console_name(enum nsk_console_type type) {
    static const char separator[] = " / ";
    static const struct nsk_multimap map[] = {
#       define X(key, name, value, description) \
        { NSK_CONSOLE_ ## name, description },

#           include <xconstants/nsk_xconst_console.x>

#       undef X

        { 0, NULL }
    };

    return nsk_multimap_lookup(map, type, separator);
}