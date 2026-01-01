#if defined(NSK_OS_WINDOWS)

#include <string.h>

#include "../../error/nsk_util_errno.h"

/*!
 * \brief  Returns the text message related to the provided code
 *
 * \param[in] errnum  The errno code
 * \return Static string
 */
const char *nsk_util_strerror(unsigned errnum) {
    /*
     * This version is thread-safe
     */
    return strerror(errnum);
}

#endif