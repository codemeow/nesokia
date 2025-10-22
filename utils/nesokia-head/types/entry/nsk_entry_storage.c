#include <stddef.h>

#include "../../types/entry/nsk_entry_storage.h"

/*!
 * \brief  Storage for the scanned entries
 */
struct nsk_entry *nsk_entry_storage = NULL;

/*!
 * \brief Module destructor
 */
__attribute__((destructor))
static void _fini(void) {
    nsk_entry_free(nsk_entry_storage);
}