#include <string.h>
#include <stdlib.h>

#include "../../types/pair/nsk_pair_cmp.h"

#include "../../utils/log/nsk_log_err.h"
#include "../../utils/nsk_util_size.h"

/* Forward declaration */
static bool _compare_equal      (uint64_t left, uint64_t right);
static bool _compare_notequal   (uint64_t left, uint64_t right);
static bool _compare_more       (uint64_t left, uint64_t right);
static bool _compare_moreorequal(uint64_t left, uint64_t right);
static bool _compare_less       (uint64_t left, uint64_t right);
static bool _compare_lessorequal(uint64_t left, uint64_t right);

/*!
 * Operators table
 */
static const struct {
    const char *string;          /*!< String representation of the operator   */
    enum nsk_pair_operator operator;  /*!< Comparison operator                */
    bool (*function)(uint64_t left, uint64_t right); /*!< Comparison callback */
} nsk_compare_table[] = {
    { "=",  NSK_PAIR_EQUAL,         _compare_equal          },
    { "!=", NSK_PAIR_NOTEQUAL,      _compare_notequal       },
    { "<>", NSK_PAIR_NOTEQUAL,      _compare_notequal       },
    { ">",  NSK_PAIR_MORE,          _compare_more           },
    { ">=", NSK_PAIR_MOREOREQUAL,   _compare_moreorequal    },
    { "<",  NSK_PAIR_LESS,          _compare_less           },
    { "<=", NSK_PAIR_LESSOREQUAL,   _compare_lessorequal    }
};

/*!
 * Size of the operators table
 */
static const size_t nsk_compare_tablesize = NSK_SIZE(nsk_compare_table);

/*!
 * \brief  Check if left == right
 *
 * \param[in] left   The left value
 * \param[in] right  The right value
 * \return True if so
 */
static bool _compare_equal(uint64_t left, uint64_t right) {
    return left == right;
}

/*!
 * \brief  Check if left != right
 *
 * \param[in] left   The left value
 * \param[in] right  The right value
 * \return True if so
 */
static bool _compare_notequal(uint64_t left, uint64_t right) {
    return left != right;
}

/*!
 * \brief  Check if left > right
 *
 * \param[in] left   The left value
 * \param[in] right  The right value
 * \return True if so
 */
static bool _compare_more(uint64_t left, uint64_t right) {
    return left > right;
}

/*!
 * \brief  Check if left >= right
 *
 * \param[in] left   The left value
 * \param[in] right  The right value
 * \return True if so
 */
static bool _compare_moreorequal(uint64_t left, uint64_t right) {
    return left >= right;
}

/*!
 * \brief  Check if left < right
 *
 * \param[in] left   The left value
 * \param[in] right  The right value
 * \return True if so
 */
static bool _compare_less(uint64_t left, uint64_t right) {
    return left < right;
}

/*!
 * \brief  Check if left <= right
 *
 * \param[in] left   The left value
 * \param[in] right  The right value
 * \return True if so
 */
static bool _compare_lessorequal(uint64_t left, uint64_t right) {
    return left <= right;
}

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
) {
    switch (operator) {
        case NSK_PAIR_EQUAL         :
        case NSK_PAIR_NOTEQUAL      :
        case NSK_PAIR_LESS          :
        case NSK_PAIR_LESSOREQUAL   :
        case NSK_PAIR_MORE          :
        case NSK_PAIR_MOREOREQUAL   :
            return nsk_compare_table[operator].function(left, right);
    }

    nsk_err("Error: unknown compare operator: %d\n", (int)operator);
    abort();
}

/*!
 * \brief  Returns string representation of the operator
 *
 * \param[in] operator The operator
 * \return Static string
 */
const char *nsk_pair_operatorstring(enum nsk_pair_operator operator) {
    for (size_t i = 0; i < nsk_compare_tablesize; i++) {
        if (nsk_compare_table[i].operator == operator) {
            return nsk_compare_table[i].string;
        }
    }

    nsk_err("Error: unknown compare operator: %d\n", (int)operator);
    abort();
}

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
) {
    size_t maxsize = 0;
    enum nsk_pair_operator maxoperator;

    for (size_t i = 0; i < nsk_compare_tablesize; i++) {
        unsigned long len = strlen(nsk_compare_table[i].string);
        int cmp = strncmp(
            nsk_compare_table[i].string,
            string,
            len
        );

        if (cmp == 0) {
            if (len > maxsize) {
                maxsize = len;
                maxoperator = nsk_compare_table[i].operator;
            }
        }
    }

    if (maxsize != 0) {
        *size = maxsize;
        *operator = maxoperator;
        return true;

    } else {
        return false;
    }
}
