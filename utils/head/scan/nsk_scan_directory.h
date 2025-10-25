#ifndef NSK_SCAN_DIRECTORY
#define NSK_SCAN_DIRECTORY

/*!
 * \brief  Scans the provided directory and processes every found entry
 *
 * \param[in] directory  The directory
 * \param[in] level      The deepness level (starts with 0)
 */
void nsk_scan_directory(const char *directory, unsigned level);

#endif