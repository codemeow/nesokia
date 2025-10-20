#ifndef NSK_PAIR_CMP
#define NSK_PAIR_CMP

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 * \brief  Match function possible oparators
 */
enum nsk_pair_operator {
    NSK_PAIR_EQUAL,        /*!< =, equal               */
    NSK_PAIR_NOTEQUAL,     /*!< !=, not equal          */
    NSK_PAIR_LESS,         /*!< <, less than           */
    NSK_PAIR_LESSOREQUAL,  /*!< <=, less or equal      */
    NSK_PAIR_MORE,         /*!< >, more than           */
    NSK_PAIR_MOREOREQUAL   /*!< >=, more or equal      */
};

/*!
 * \brief  Values comparator
 *
 * \param[in] left       The left value
 * \param[in] right      The right value
 * \param[in] operator   The operator
 * \return True if <left> <operator> <right>, i.e. 304 < 600
 */
bool nsk_pair_compare(
    uint64_t left,
    uint64_t right,
    enum nsk_pair_operator operator
);

/*!
 * \brief  Returns string representation of the operator
 *
 * \param[in] operator The operator
 * \return Static string
 */
const char *nsk_pair_operatorstring(enum nsk_pair_operator operator);

/*!
 * \brief  Checks if the provided string contains valid operator, i.e. '<'
 *
 * \param[in] string    The string
 * \param[out] size      The size of operator
 * \param[out] operator Parsed operator
 * \return True if the string starts with a valid operator
 */
bool nsk_pair_isoperator(
    const char *string,
    size_t *size,
    enum nsk_pair_operator *operator
);

#endif