#ifndef NSK_HEADER_RAW2DATA
#define NSK_HEADER_RAW2DATA

#include "../../types/header/nsk_header_data.h"
#include "../../types/header/nsk_header_raw.h"

/*!
 * \brief  Converts raw header to parsed data
 *
 * \param[in] raw  The raw header values
 * \param[out] data Parsed header data
 */
void nsk_header_raw2data(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
);

#endif