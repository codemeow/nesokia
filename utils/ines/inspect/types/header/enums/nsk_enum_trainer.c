#include <stdlib.h>

#include "../../../types/header/enums/nsk_enum_trainer.h"

#include "../../../utils/nsk_util_multimap.h"

/*!
 * \brief  Returns text description of the type
 *
 * \param[in] type  The trainer type
 * \return Static string
 */
const char *nsk_trainer_name(enum nsk_trainer_type type) {
    static const char separator[] = " / ";
    static const struct nsk_multimap map[] = {
#       define X(key, name, value, description) \
        { NSK_TRAINER_ ## name, description },

#           include <xconstants/nsk_xconst_trainer.x>

#       undef X

        { 0, NULL }
    };

    return nsk_multimap_lookup(map, type, separator);
}
