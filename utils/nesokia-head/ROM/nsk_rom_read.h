#ifndef NSK_ROM_READ
#define NSK_ROM_READ

#include <stdbool.h>

#include "../ROM/header/nsk_header_raw.h"

/*!
 * \brief  Read the header from the ROM file
 *
 * \param[in] filename  The filename
 * \param[out] header    The header
 * \return True on success
 */
bool nsk_rom_read(const char *filename, struct nsk_header_raw *header);

#endif