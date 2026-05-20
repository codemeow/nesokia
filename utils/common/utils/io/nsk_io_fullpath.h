#ifndef NSK_IO_FULLPATH
#define NSK_IO_FULLPATH

#include "base/nsk_util_attributes.h"

/*!
 * \brief Program name passed to as argv[0]
 */
extern const char *nsk_args_program;

/*!
 * \brief  Resolves relative path to the full path starting with the current
 * binary position
 *
 * \param[in] path  The relative path
 * \return Allocated string with full resolved path
 */
nsk_attr_result_unused
nsk_attr_args_nonnull(1)
char *nsk_io_fullpath(const char *path);

#endif
