#ifndef NSK_IO_FOPEN
#define NSK_IO_FOPEN

#include <stdio.h>

#include "base/nsk_util_attributes.h"

/*!
 * \brief  Platform-specific version of fopen
 *
 * \param[in] filename  The filename
 * \param[in] mode      The mode
 * \return Opened file or NULL
 */
nsk_attr_result_unused
nsk_attr_args_nonnull(1, 2)
FILE *nsk_io_fopen(
    const char *filename,
    const char *mode
);

#endif
