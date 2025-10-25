#ifndef NSK_ENTRY_MATCH
#define NSK_ENTRY_MATCH

#include <stdbool.h>

#include "../../types/header/nsk_header_data.h"

/*!
 * \brief  Checks if the provided header matches the set filters
 *
 * \param[in]  header  The header
 * \return True if the header matches the provided filters
 */
bool nsk_entry_match(const struct nsk_header_data *header);

#endif