#if defined(NSK_OS_WINDOWS)

#include "../../io/nsk_io_fopen.h"

#include "../../strings/windows/nsk_strings_wide.h"
#include "../../utils/nsk_util_cleanup.h"
#include "../../utils/nsk_util_malloc.h"

/*!
 * \brief  Platform-specific version of fopen
 *
 * \param[in] filename  The filename
 * \param[in] mode      The mode
 * \return Opened file or NULL
 */
FILE *nsk_io_fopen(const char *filename, const char *mode) {
    nsk_auto_free LPWSTR wpath = nsk_string_a2w(filename);
    nsk_auto_free LPWSTR wmode = nsk_string_a2w(mode);

    FILE * stream = _wfopen(wpath, wmode);

    return stream;
}

#endif