#ifndef NSK_SCAN_LINK
#define NSK_SCAN_LINK

#include <stdbool.h>

/*!
 * \brief  Scan the symbolic link entry
 *
 * \param[in] linkname  The link filename
 * \param[in] level      Deepness level
 * \return True if at least one valid ROM was read
 */
bool nsk_scan_link(const char *linkname, unsigned level);

#endif
