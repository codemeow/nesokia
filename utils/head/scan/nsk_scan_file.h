#ifndef NSK_SCAN_FILE
#define NSK_SCAN_FILE

/*!
 * \brief  Scans the provided file and either provides detailed description
 * or puts it into table's stack
 *
 * \param[in] filename  The filename
 * \param[in] level      Deepness level
 */
void nsk_scan_file(const char *filename, unsigned level);

#endif