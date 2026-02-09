#ifndef NSK_ENTRY_TYPE
#define NSK_ENTRY_TYPE

#include <nsk_util_meta.h>

#include "../../types/header/nsk_header_data.h"

/*!
 * \brief  Parsed entry data list
 */
struct nsk_entry {
    char *filename;              /*!< Filename           */
    struct nsk_header_data data; /*!< Parsed header data */

    struct nsk_list list;        /*!< List element       */
};

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
);

/*!
 * \brief  Frees resources of the parent (provided) element and all connected
 * child elements
 *
 * \param[in, out]  entry  The entry
 */
void nsk_entry_free(struct nsk_entry *entry);

/*!
 * \brief  Provides the next element of the entry list
 *
 * \param[in] entry  The entry
 * \return Next element or NULL
 */
struct nsk_entry *nsk_entry_next(const struct nsk_entry *entry);

#endif