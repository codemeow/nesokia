#ifndef NSK_SCAN_ENTRY
#define NSK_SCAN_ENTRY

#include <stdbool.h>

/*!
 * \brief  Scans the provided entry and either provides detailed description
 * or puts it into table's stack
 *
 * \param[in] entryname  The entry file-/directoryname
 * \param[in] level      Deepness level
 * \return True if at least one valid ROM was read
 */
bool nsk_scan_entry(const char *entryname, unsigned level);

#endif
