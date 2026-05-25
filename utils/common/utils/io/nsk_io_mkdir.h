#ifndef NSK_IO_MKDIR
#define NSK_IO_MKDIR

#include <sys/types.h>

#include "base/nsk_util_attributes.h"

/*!
 * \brief  Creates the path
 *
 * \param[in] path  The path
 * \param[in] mode  The mode
 * \return 0 on success, -1 on error with errno being set
 */
nsk_attr_result_unused
nsk_attr_args_nonnull(1)
int nsk_io_mkdir(
    const char *path,
    mode_t mode
);

#endif
