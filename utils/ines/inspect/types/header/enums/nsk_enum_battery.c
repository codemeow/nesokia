#include <stdlib.h>
#include <nsk_util_meta.h>

#include "../../../types/header/enums/nsk_enum_battery.h"


/*!
 * \brief  Returns text description of the type
 *
 * \param[in] type  The battery type
 * \return Static string
 */
const char *nsk_battery_name(enum nsk_battery_type type) {
    static const char separator[] = " / ";
    static const struct nsk_multimap map[] = {
#       define X(key, name, value, description) \
        { NSK_BATTERY_ ## name, description },

#           include <xconstants/nsk_xconst_battery.x>

#       undef X

        { 0, NULL }
    };

    return nsk_multimap_lookup(map, type, separator);
}
