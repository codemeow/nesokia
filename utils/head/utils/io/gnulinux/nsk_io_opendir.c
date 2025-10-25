#if defined (NSK_OS_GNULINUX)

#include <dirent.h>
#include <sys/types.h>

#include "../../io/nsk_io_opendir.h"

/*!
 * \brief  Opens the directory with platform-specific features
 *
 * \param[in] path  The path
 * \return Opened directory or NULL
 */
DIR *nsk_io_opendir(const char *path) {
    return opendir(path);
}

#endif