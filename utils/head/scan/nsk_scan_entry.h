#ifndef NSK_SCAN_ENTRY
#define NSK_SCAN_ENTRY

/*!
 * \brief  Scans the provided entry and either provides detailed description
 * or puts it into table's stack
 *
 * \param[in] entryname  The entry file-/directoryname
 * \param[in] level      Deepness level
 */
void nsk_scan_entry(const char *entryname, unsigned level);

#endif