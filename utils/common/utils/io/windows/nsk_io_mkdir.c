#if defined(NSK_OS_WINDOWS)

#include <sys/stat.h>
#include <windows.h>

#include "../../io/nsk_io_mkdir.h"
#include "../../strings/windows/nsk_strings_wide.h"
#include "../../nsk_util_cleanup.h"

/*!
 * \brief  Creates the path
 *
 * \param[in] path  The path
 * \param[in] mode  The mode
 * \return 0 on success, -1 on error with errno being set
 */
int nsk_io_mkdir(
    const char *path,
    mode_t mode         __attribute__((unused))
) {
    nsk_auto_free LPWSTR wpath = nsk_string_a2w(path);
    return _wmkdir(wpath);
}

#endif