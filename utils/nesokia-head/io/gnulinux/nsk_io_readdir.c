#if defined(NSK_OS_GNULINUX)

#include <sys/types.h>
#include <dirent.h>

#include "../../io/nsk_io_readdir.h"

/*!
 * \brief  Return next element of the directory
 *
 * \param[in]  dir  The dir
 * \return Next element or NULL
 */
struct dirent *nsk_io_readdir(DIR *dir) {
    return readdir(dir);
}

#endif