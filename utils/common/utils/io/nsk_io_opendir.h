#ifndef NSK_IO_OPENDIR
#define NSK_IO_OPENDIR

#include <dirent.h>

#include "base/nsk_util_attributes.h"

/*!
 * \brief  Opens the directory with platform-specific features
 *
 * \param[in] path  The path
 * \return Opened directory or NULL
 */
nsk_attr_result_unused
nsk_attr_args_nonnull(1)
DIR *nsk_io_opendir(
    const char *path
);

#endif
