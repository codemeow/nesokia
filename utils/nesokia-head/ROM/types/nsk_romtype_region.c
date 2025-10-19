#include <stddef.h>

#include "../../ROM/types/nsk_romtype_region.h"
#include "../../utils/nsk_util_multimap.h"

/*!
 * \brief  Returns string description of the region type
 *
 * \param[in] type  The type
 * \return  Static string
 */
const char *nsk_region_name(enum nsk_region_type type) {
    static const char separator[] = " / ";
    static const struct nsk_multimap map[] = {
#       define X(key, name, value, description) \
        { NSK_REGION_ ## name, description },

#           include "../../../xconstants/nsk_xconst_region.x"

#       undef X

        { 0, NULL }
    };

    return nsk_multimap_lookup(map, type, separator);
}