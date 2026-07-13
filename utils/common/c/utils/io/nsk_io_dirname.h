#ifndef NSK_IO_DIRNAME
#define NSK_IO_DIRNAME

#include "base/nsk_util_attributes.h"

/*!
 * \brief  Returns the directory path of the provided filename
 *
 * \param[in] filename  The filename
 * \return Allocated string
 */
nsk_attr_result_unused
nsk_attr_args_nonnull(1)
char *nsk_io_dirname(const char *filename);

#endif
