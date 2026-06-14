#pragma once

#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    #define NSK_END_IS_BE 1
    #define NSK_END_IS_LE 0
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define NSK_END_IS_BE 0
    #define NSK_END_IS_LE 1
#elif defined(_WIN32) || defined(_MSC_VER)
    /* Windows is always LE */
    #define NSK_END_IS_BE 0
    #define NSK_END_IS_LE 1
#else
    /* No fallback */
    #error "Cannot detect system endianness"
#endif