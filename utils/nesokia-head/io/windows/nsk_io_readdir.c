#if defined(NSK_OS_WINDOWS)

#include <stdio.h>

#include "../../io/nsk_io_readdir.h"
#include "../../strings/windows/nsk_strings_wide.h"
#include "../../utils/nsk_util_malloc.h"
#include "../../utils/nsk_util_cleanup.h"

/*!
 * \brief  Return next element of the directory
 *
 * \param[in]  dir  The dir
 * \return Next element or NULL
 */
struct dirent *nsk_io_readdir(DIR *dir) {
    /*
     * See the commentary in nsk_io_opendir.c (windows version)
     */

    struct dirent * result = readdir(dir);
    if (!result) {
        return NULL;
    }

    nsk_auto_free LPWSTR wname = nsk_string_a2w_enc(
        result->d_name,
        CP_ACP,
        MB_ERR_INVALID_CHARS
    );
    nsk_auto_free char * name = nsk_string_w2a(wname);

    int ret = snprintf(result->d_name, sizeof(result->d_name), "%s", name);

    if (ret < 0 || (size_t)ret >= sizeof(result->d_name)) {
        return NULL;
    }

    return result;
}

#endif