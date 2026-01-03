#if defined (NSK_OS_WINDOWS)

#include "../../io/nsk_io_opendir.h"

#include "../../strings/windows/nsk_strings_wide.h"
#include "../../nsk_util_cleanup.h"
#include "../../nsk_util_malloc.h"

/*!
 * \brief  Opens the directory with platform-specific features
 *
 * \param[in] path  The path
 * \return Opened directory or NULL
 */
DIR *nsk_io_opendir(const char *path) {
    /*
     * This double conversion (ANSI/UTF-8 → UTF-16 → ANSI) is used to normalize
     * path encoding on Windows. The input may be in UTF-8 or another multibyte
     * format, while opendir() expects a path encoded in the current ANSI code
     * page. By first converting to wide characters (UTF-16) and then back to
     * ANSI using a controlled code page (CP_ACP), we ensure consistent, locale-
     * safe behavior and avoid broken file names with non-ASCII characters.
     */
    nsk_auto_free LPWSTR wpath = nsk_string_a2w(path);
    nsk_auto_free char * apath = nsk_string_w2a_enc(
        wpath,
        CP_ACP,
        WC_NO_BEST_FIT_CHARS
    );

    DIR * stream = opendir(apath);

    return stream;
}

#endif