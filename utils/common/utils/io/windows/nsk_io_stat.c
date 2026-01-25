#if defined(NSK_OS_WINDOWS)

#include <stdbool.h>
#include <sys/stat.h>
#include <time.h>
#include <windows.h>

#include "../../io/nsk_io_stat.h"

#include "../../log/nsk_log_err.h"
#include "../../strings/windows/nsk_strings_wide.h"
#include "../../nsk_util_cleanup.h"
#include "../../nsk_util_malloc.h"

/*!
 * \brief  Executes platform-specific `stat`
 *
 * \param[in] path     The path
 * \param[out] statbuf  The statbuf
 * \return 0 at success
 */
int nsk_io_stat(const char *path, struct stat *statbuf) {
    nsk_auto_free LPWSTR wpath = nsk_string_a2w(path);

    int result = wstat(wpath, statbuf);

    return result;
}

/*!
 * \brief  Provides platform-specific lstat
 *
 * \param[in]  path  The entry path
 * \param[out] stat  The stat buffer
 * \return 0 at success
 */
int nsk_io_lstat(const char *path, struct stat *statbuf) {
    /* Windows symlinks aren't common and supporting them
     * would require too much of the code */
    return nsk_io_stat(path, statbuf);
}

#endif