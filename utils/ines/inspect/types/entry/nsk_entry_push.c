#include <stddef.h>

#include "../../types/entry/nsk_entry_push.h"
#include "../../types/entry/nsk_entry_storage.h"

/*!
 * Last entry record
 */
static struct nsk_entry *nsk_entry_last = NULL;

/*!
 * \brief  Pushes te new entry to the storage
 *
 * \param[in] filename  The filename
 * \param[in] data      The header data
 */
void nsk_entry_push(const char *filename, const struct nsk_header_data *data) {
    nsk_entry_last = nsk_entry_append(filename, data, nsk_entry_last);

    if (!nsk_entry_storage) {
        nsk_entry_storage = nsk_entry_last;
    }
}