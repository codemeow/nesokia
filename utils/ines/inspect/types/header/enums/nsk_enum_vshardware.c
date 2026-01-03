#include <stddef.h>
#include <nsk_util_meta.h>

#include "../../../types/header/enums/nsk_enum_vshardware.h"

/*!
 * \brief  Returns string description of the Vs. hardware
 *
 * \param[in] type  The type
 * \return  Static string
 */
const char *nsk_vshardware_name(enum nsk_vshardware_type type) {
    static const char separator[] = " / ";
    static const struct nsk_multimap map[] = {
#       define X(key, name, value, description) \
        { NSK_VSHARDWARE_ ## name, description },

#           include <xconstants/nsk_xconst_vshardware.x>

#       undef X

        { 0, NULL }
    };

    return nsk_multimap_lookup(map, type, separator);
}