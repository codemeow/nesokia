#ifndef NSK_UTIL_ASSERT_H
#define NSK_UTIL_ASSERT_H

#include <assert.h>

/*!
 * \brief  Provides the standard-independend static assert macro
 *
 * \param[in]  cond  The condition
 * \param[in]  msg   The message to print
 */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    #define NSK_STATIC_ASSERT(cond, msg) static_assert(cond, msg)

#else
    #define NSK_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
#endif

#endif