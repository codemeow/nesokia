#include <stddef.h>

#include "../list/nsk_list_type.h"

/*!
 * \brief  Initializes the list element;
 *
 * \param[in]  The element to initialize
 */
void nsk_list_init(struct nsk_list *list) {
    list->next = NULL;
}

/*!
 * \brief  Connects child element to the parent
 *
 * \param[in]  parent  The parent
 * \param[in]  child   The child
 */
void nsk_list_append(struct nsk_list *parent, struct nsk_list *child) {
    if (parent) {
        parent->next = child;
    }
}