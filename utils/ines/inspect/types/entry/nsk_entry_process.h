#ifndef NSK_ENTRY_PROCESS
#define NSK_ENTRY_PROCESS

#include <stdbool.h>

/*!
 * \brief  Processes single file
 *
 * \param[in] filename  The filename
 * \return True if the file was read as a valid ROM
 */
bool nsk_entry_process(const char *filename);

#endif
