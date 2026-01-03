#ifndef NSK_IO_MKDIRP
#define NSK_IO_MKDIRP

#include <sys/types.h>

/*!
 * \brief  Creates the path including all subdirectories
 *
 * \param[in] path  The path
 * \param[in] mode  The mode
 * \return 0 on success, -1 on error with errno being set
 */
int nsk_io_mkdirp(const char *path, mode_t mode);

#endif