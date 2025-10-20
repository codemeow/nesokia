#ifndef NSK_PAIR_TYPE
#define NSK_PAIR_TYPE

#include <stdint.h>

#include "../../list/nsk_list_type.h"
#include "../../list/pair/nsk_pair_cmp.h"

/*!
 * \brief  Name:value pair holder
 */
struct nsk_pair {
    /*!
     * @brief Name of the field
     *
     * Refer to `(struct nsk_header_tableentry).shortcut`
     */
    char                   *name;
    uint64_t                value;    /*!< Value of the field  */
    enum nsk_pair_operator  operator; /*!< Comparison operator */

    struct nsk_list list;   /*!< List element */
};

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
);

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
);

/*!
 * \brief  Frees resources of the parent (provided) element and all connected
 * child elements
 *
 * \param[in, out]  pair  The pair
 */
void nsk_pair_free(struct nsk_pair *pair);

/*!
 * \brief  Provides the next element of the pair
 *
 * \param[in]  pair  The pair
 * \return Next element or NULL
 */
struct nsk_pair *nsk_pair_next(const struct nsk_pair *pair);

#endif