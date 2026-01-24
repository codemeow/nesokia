#if defined(NSK_MODULE_COMPOSE)

#include <stdlib.h>

#include "../types/nsk_type_plane.h"
#include "../log/nsk_log_err.h"

/*!
 * \brief  Converts the plane enum into string description
 *
 * \param[in]  value  The value
 * \return Static string
 */
const char *nsk_conv_plane2string(enum nsk_plane_list value) {
    switch (value) {
        case NSK_PLANE_BACKGROUND: return "Background";
        case NSK_PLANE_SPRITES   : return "Sprites";

        default:
            nsk_err(
                "Invalid plane value to convert: %d\n",
                (int)value
            );
            abort();
    }
}

#endif