#include <string.h>

#include "../../list/pair/nsk_pair_type.h"
#include "../../utils/nsk_util_malloc.h"
#include "../../strings/nsk_strings_strdup.h"

/*!
 * \brief  Creates and appends new element to the provided optional list
 *
 * \param[in] name          The pair name
 * \param[in] value         The pair value
 * \param[in] operator      The pair operator
 * \param[in, out] parent   The parent list (optional)
 * \return Freshly allocated pair element
 */
struct nsk_pair *nsk_pair_append(
    const char      *name,
    uint64_t         value,
    enum nsk_pair_operator operator,
    struct nsk_pair *parent
) {
    struct nsk_pair *pair = nsk_util_malloc(sizeof(*pair));

    pair->name  = nsk_strings_strdup(name);
    pair->value = value;
    pair->operator = operator;
    if (parent) {
        nsk_list_append(&parent->list, &pair->list);
    }

    return pair;
}

/*!
 * \brief  Creates and appends new element to the provided optional list
 *
 * \param[in] name          The pair name
 * \param[in] value         The pair value
 * \param[in] operator      The pair operator
 * \param[in, out] parent   The parent list (optional)
 * \return Freshly allocated pair element
 */
struct nsk_pair *nsk_pair_appendn(
    const char      *name,
    size_t           namesize,
    uint64_t         value,
    enum nsk_pair_operator operator,
    struct nsk_pair *parent
) {
    char namebuf[namesize + 1];
    memcpy(namebuf, name, namesize);
    namebuf[namesize] = '\0';

    return nsk_pair_append(namebuf, value, operator, parent);
}

/*!
 * \brief  Frees resources of the parent (provided) element and all connected
 * child elements
 *
 * \param[in, out]  pair  The pair
 */
void nsk_pair_free(struct nsk_pair *pair) {
    if (!pair) {
        return;
    }

    nsk_pair_free(nsk_pair_next(pair));

    nsk_util_free(pair->name);
    nsk_util_free(pair);
}

/*!
 * \brief  Provides the next element of the pair
 *
 * \param[in]  pair  The pair
 * \return Next element or NULL
 */
struct nsk_pair *nsk_pair_next(const struct nsk_pair *pair) {
    if (!pair) {
        return NULL;
    }

    struct nsk_list *next = pair->list.next;
    if (!next) {
        return NULL;
    }

    return NSK_CONTAINER(next, struct nsk_pair, list);
}