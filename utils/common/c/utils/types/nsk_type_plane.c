#include <stddef.h>

#include "types/nsk_type_plane.h"

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
        case NSK_PLANES_COUNT    : return NULL;
    }

    __builtin_unreachable();
}
