#include <stdlib.h>
#include <nsk_util_meta.h>

#include "../../../types/header/enums/nsk_enum_alternative.h"


/*!
 * \brief  Returns text description of the type
 *
 * \param[in] type  The alternative type
 * \return Static string
 */
const char *nsk_alternative_name(enum nsk_alternative_type type) {
    static const char separator[] = " / ";
    static const struct nsk_multimap map[] = {
#       define X(key, name, value, description) \
        { NSK_ALTERNATIVE_ ## name, description },

#           include <xconstants/nsk_xconst_alternative.x>

#       undef X

        { 0, NULL }
    };

    return nsk_multimap_lookup(map, type, separator);
}
