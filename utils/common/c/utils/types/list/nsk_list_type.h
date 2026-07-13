#ifndef NSK_LIST_TYPE
#define NSK_LIST_TYPE

#include <stddef.h>

/*!
 * \brief  Returns the pointer to the parental container by the provided
 * fields.
 * Examples of use:
 * ~~~
 * struct container {
 *     int data;
 *     struct nsk_list next;
 * }
 *
 * struct container container = <init>;
 * struct nsk_list *list = &container.list;
 * ...
 * struct container *ptr = NSK_CONTAINER(list, struct container, next);
 * ~~~
 */
#define NSK_CONTAINER(list, containertype, name) \
    ((containertype *)((char *)(list) - offsetof(containertype, name)))

/*!
 * \brief  Singly linked list implementation
 */
struct nsk_list {
    struct nsk_list *next;
};

/*!
 * \brief  Initializes the list element;
 *
 * \param[in]  The element to initialize
 */
void nsk_list_init(struct nsk_list *list);

/*!
 * \brief  Connects child element to the parent
 *
 * \param[in]  parent  The parent
 * \param[in]  child   The child
 */
void nsk_list_append(struct nsk_list *parent, struct nsk_list *child);

#endif