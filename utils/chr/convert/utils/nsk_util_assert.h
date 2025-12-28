#ifndef NSK_UTIL_ASSERT_H
#define NSK_UTIL_ASSERT_H

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    #define NSK_STATIC_ASSERT(cond, msg) static_assert(cond, msg)

#else
    #include <assert.h>

    #define NSK_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
#endif

#endif