#ifndef NSK_IO_OPENDIR
#define NSK_IO_OPENDIR

#include <dirent.h>

/*!
 * \brief  Opens the directory with platform-specific features
 *
 * \param[in] path  The path
 * \return Opened directory or NULL
 */
DIR *nsk_io_opendir(const char *path);

#endif