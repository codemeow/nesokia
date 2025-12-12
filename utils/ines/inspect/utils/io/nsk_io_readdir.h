#ifndef NSK_IO_READDIR
#define NSK_IO_READDIR

#include <dirent.h>

/*!
 * \brief  Return next element of the directory
 *
 * \param[in]  dir  The dir
 * \return Next element or NULL
 */
struct dirent *nsk_io_readdir(DIR *dir);

#endif