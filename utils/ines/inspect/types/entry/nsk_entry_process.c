#include "../../types/entry/nsk_entry_process.h"
#include "../../types/entry/nsk_entry_match.h"
#include "../../types/entry/nsk_entry_push.h"
#include "../../types/entry/nsk_entry_read.h"
#include "../../types/header/nsk_header_data.h"
#include "../../types/header/nsk_header_raw2data.h"

/*!
 * \brief  Processes single file
 *
 * \param[in] filename  The filename
 */
void nsk_entry_process(const char *filename) {
    struct nsk_header_raw raw;
    struct nsk_header_data data;

    if (!nsk_entry_read(filename, &raw)) {
        return;
    }

    nsk_header_raw2data(&raw, &data);

    if (nsk_entry_match(&data)) {
        nsk_entry_push(filename, &data);
    }
}