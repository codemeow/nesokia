#include <nsk_util_meta.h>

#include "../../types/entry/nsk_entry_type.h"


/*!
 * \brief  Creates and appends new entry to the provided optional parent
 *
 * \param[in] filename  The filename
 * \param[in] data      The data
 * \param[in] parent    The parent (optional)
 * \return Freshly allocated entry element
 */
struct nsk_entry *nsk_entry_append(
    const char *filename,
    const struct nsk_header_data *data,
    struct nsk_entry *parent
) {
    struct nsk_entry *entry = nsk_util_malloc(sizeof(*entry));

    entry->filename = nsk_strings_strdup(filename);
    entry->data     = *data;

    if (parent) {
        nsk_list_append(&parent->list, &entry->list);
    }

    return entry;
}

/*!
 * \brief  Frees resources of the parent (provided) element and all connected
 * child elements
 *
 * \param[in, out]  entry  The entry
 */
void nsk_entry_free(struct nsk_entry *entry) {
    if (!entry) {
        return;
    }

    nsk_entry_free(nsk_entry_next(entry));
    nsk_util_free(entry->filename);
    nsk_util_free(entry);
}

/*!
 * \brief  Provides the next element of the entry list
 *
 * \param[in] entry  The entry
 * \return Next element or NULL
 */
struct nsk_entry *nsk_entry_next(const struct nsk_entry *entry) {
    if (!entry) {
        return NULL;
    }

    struct nsk_list *next = entry->list.next;
    if (!next) {
        return NULL;
    }

    return NSK_CONTAINER(next, struct nsk_entry, list);
}
