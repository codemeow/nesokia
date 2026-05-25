#ifndef NSK_SCAN_DIRECTORY
#define NSK_SCAN_DIRECTORY

#include <stdbool.h>

/*!
 * \brief  Scans the provided directory and processes every found entry
 *
 * \param[in] directory  The directory
 * \param[in] level      The deepness level (starts with 0)
 * \return True if at least one valid ROM was read
 */
bool nsk_scan_directory(const char *directory, unsigned level);

#endif
