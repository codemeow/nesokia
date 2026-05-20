#ifndef NSK_IO_READDIR
#define NSK_IO_READDIR

#include <dirent.h>

#include "base/nsk_util_attributes.h"

/*!
 * \brief  Return next element of the directory
 *
 * \param[in]  dir  The dir
 * \return Next element or NULL
 */
nsk_attr_result_unused
nsk_attr_args_nonnull(1)
struct dirent *nsk_io_readdir(
    DIR *dir
);

#endif
