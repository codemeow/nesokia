#include <stdlib.h>
#include <nsk_util_meta.h>

#include "../../../types/header/enums/nsk_enum_mirroring.h"

/*!
 * \brief  Returns text description of the type
 *
 * \param[in] type  The mirroring type
 * \return Static string
 */
const char *nsk_mirroring_name(enum nsk_mirroring_type type) {
    static const char separator[] = " / ";
    static const struct nsk_multimap map[] = {
#       define X(key, name, value, description) \
        { NSK_MIRRORING_ ## name, description },

#           include <xconstants/nsk_xconst_layout.x>

#       undef X

        { 0, NULL }
    };

    return nsk_multimap_lookup(map, type, separator);
}