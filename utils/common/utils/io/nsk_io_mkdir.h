#ifndef NSK_IO_MKDIR
#define NSK_IO_MKDIR

#include <sys/types.h>

/*!
 * \brief  Creates the path
 *
 * \param[in] path  The path
 * \param[in] mode  The mode
 * \return 0 on success, -1 on error with errno being set
 */
int nsk_io_mkdir(const char *path, mode_t mode);

#endif