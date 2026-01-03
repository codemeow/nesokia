#ifndef NSK_IO_TESTD
#define NSK_IO_TESTD

#include <stdbool.h>

/*!
 * \brief  Tests the directory for existence.
 *
 * \param[in] path  The path
 * \return True if the directory exists and accessible
 */
bool nsk_io_testd(const char *path);

#endif