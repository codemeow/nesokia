#include <stddef.h>

#include "../../ROM/types/nsk_romtype_device.h"
#include "../../utils/nsk_util_multimap.h"

/*!
 * \brief  Returns string description of the expansion device
 *
 * \param[in] type  The type
 * \return  Static string
 */
const char *nsk_device_name(enum nsk_device_type type) {
    static const char separator[] = " / ";
    static const struct nsk_multimap map[] = {
#       define X(key, manufacturer, device, value, description) \
        { NSK_DEVICE_ ## manufacturer ## _ ## device, description },

#           include "../../../xconstants/nsk_xconst_device.x"

#       undef X

        { 0, NULL }
    };

    return nsk_multimap_lookup(map, type, separator);
}