#include <stddef.h>

#include "../../ROM/header/nsk_header_data.h"
#include "../../ROM/header/nsk_header_table.h"

/*!
 * \brief  Converts raw header to parsed data
 *
 * \param[in] raw  The raw header values
 * \return Parsed header data
 */
struct nsk_header_data nsk_header_raw2data(const struct nsk_header_raw *raw) {
    struct nsk_header_data data;

    for (size_t i = 0; i < nsk_header_tablesize; i++) {
        nsk_header_table[i].conv_raw(raw, &data);
    }

    return data;
}