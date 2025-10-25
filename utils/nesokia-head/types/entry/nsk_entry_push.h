#ifndef NSK_ENTRY_PUSH
#define NSK_ENTRY_PUSH

#include "../../types/header/nsk_header_data.h"

/*!
 * \brief  Pushes te new entry to the storage
 *
 * \param[in] filename  The filename
 * \param[in] data      The header data
 */
void nsk_entry_push(const char *filename, const struct nsk_header_data *data);

#endif