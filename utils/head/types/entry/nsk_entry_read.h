#ifndef NSK_ENTRY_READ
#define NSK_ENTRY_READ

#include <stdbool.h>

#include "../../types/header/nsk_header_raw.h"

/*!
 * \brief  Read the header from the ROM file
 *
 * \param[in] filename  The filename
 * \param[out] header    The header
 * \return True on success
 */
bool nsk_entry_read(const char *filename, struct nsk_header_raw *header);

#endif