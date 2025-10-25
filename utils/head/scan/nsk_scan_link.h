#ifndef NSK_SCAN_LINK
#define NSK_SCAN_LINK

/*!
 * \brief  Scan the symbolic link entry
 *
 * \param[in] linkname  The link filename
 * \param[in] level      Deepness level
 */
void nsk_scan_link(const char* linkname, unsigned level);

#endif