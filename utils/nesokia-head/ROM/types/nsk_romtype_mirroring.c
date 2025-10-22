#include <stdlib.h>

#include "../../ROM/types/nsk_romtype_mirroring.h"
#include "../../log/nsk_log_err.h"

/*!
 * \brief  Returns text description of the type
 *
 * \param[in] type  The mirroring type
 * \return Static string
 */
const char *nsk_mirroring_name(enum nsk_mirroring_type type) {
    switch (type) {
        case NSK_MIRRORING_VERTICAL     :
            return "Vertical (or mapper controlled)";
        case NSK_MIRRORING_HORIZONTAL   :
            return "Horizontal";
    }

    nsk_err("Error: unknown mirroring type: %d\n", (int)type);
    abort();
}