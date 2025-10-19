#ifndef NSK_HEADER_SHOW
#define NSK_HEADER_SHOW

#include "../../ROM/header/nsk_header_data.h"

/*!
 * \brief  Fancy prints the header data
 *
 * \param[in] filename The ROM filename
 * \param[in]  data  The header data
 */
void nsk_header_show(
    const char *filename,
    const struct nsk_header_data *data
);

#endif