#include <stddef.h>

#include "../../../types/header/enums/nsk_enum_vsppu.h"

#include "../../../utils/nsk_util_multimap.h"

/*!
 * \brief  Returns string description of the Vs. PPU
 *
 * \param[in] type  The type
 * \return  Static string
 */
const char *nsk_vsppu_name(enum nsk_vsppu_type type) {
    static const char separator[] = " / ";
    static const struct nsk_multimap map[] = {
#       define X(key, name, value, description) \
        { NSK_VSPPU_ ## name, description },

#           include <xconstants/nsk_xconst_vsppu.x>

#       undef X

        { 0, NULL }
    };

    return nsk_multimap_lookup(map, type, separator);
}