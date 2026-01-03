#if defined(NSK_OS_GNULINUX)

#include <string.h>
#include <threads.h>

#include "../../error/nsk_util_errno.h"

/*!
 * \brief  Returns the text message related to the provided code
 *
 * \param[in] errnum  The errno code
 * \return Static string
 */
const char *nsk_util_strerror(unsigned errnum) {
    /*
     * man strerror_r:
     *   The  GNU  C  Library uses a buffer of 1024 characters for strerror().
     *   This buffer size therefore should be sufficient to avoid an ERANGE er‐
     *   ror when calling strerror_r() and strerror_l().
     */
    static thread_local char _errstr[1024];
    return strerror_r(errnum, _errstr, sizeof(_errstr));
}

#endif