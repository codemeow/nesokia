#include "../math/nsk_math_endianness.h"

/*!
 * \brief Current endianness check
 */
#if \
      defined (__BIG_ENDIAN__) || \
      defined (__ARMEB__) || defined (__THUMBEB__) || defined (__AARCH64EB__) || \
     (defined (__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__))

    #define NSK_ENDIANNESS_BE
#elif \
     defined (__LITTLE_ENDIAN__) || \
     defined (__ARMEL__) || defined (__THUMBEL__) || defined (__AARCH64EL__) || \
     (defined (__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__))

    #define NSK_ENDIANNESS_LE

#else
    #define NSK_ENDIANNESS_LE
#endif

/*!
 * \brief  Converts provided value to u32-BE
 *
 * \note Takes into account the current endianness
 *
 * \param[in] value  The value
 * \return Value in u32-BE
 */
uint32_t nsk_math_tobe32(uint32_t value) {
    #if defined (NSK_ENDIANNESS_LE)
        return nsk_math_swap32(value);
    #else
        return value;
    #endif
}

/*!
 * \brief  Converts provided value to u16-BE
 *
 * \note Takes into account the current endianness
 *
 * \param[in] value  The value
 * \return Value in u16-BE
 */
uint16_t nsk_math_tobe16(uint16_t value) {
    #if defined (NSK_ENDIANNESS_LE)
        return nsk_math_swap16(value);
    #else
        return value;
    #endif
}

/*!
 * \brief  Converts provided value to float-BE
 *
 * \note Takes into account the current endianness
 *
 * \param[in] value  The value
 * \return Value in float-BE
 */
float nsk_math_tobeflt(float value) {
    #if defined (NSK_ENDIANNESS_LE)
        return nsk_math_swapflt(value);
    #else
        return value;
    #endif
}

/*!
 * \brief  Converts provided value to the opposite endiannness
 *
 * \param[in] value  The value
 * \return Swapped value
 */
uint32_t nsk_math_swap32(uint32_t value) {
    #if defined(__GNUC__)
        return __builtin_bswap32(value);

    #elif defined (__has_builtin)
        #if __has_builtin(__builtin_bswap32)
            return __builtin_bswap32(value);
        #endif
    #endif

    return
        (value >> 24) |
        ((value >> 8) & 0x0000ff00u) |
        ((value << 8) & 0x00ff0000u) |
        (value << 24);
}

/*!
 * \brief  Converts provided value to the opposite endiannness
 *
 * \param[in] value  The value
 * \return Swapped value
 */
uint16_t nsk_math_swap16(uint16_t value) {
    #if defined(__GNUC__)
        return __builtin_bswap16(value);

    #elif defined (__has_builtin)
        #if __has_builtin(__builtin_bswap16)
            return __builtin_bswap16(value);
        #endif
    #endif

    return
        (value >> 8) |
        (value << 8);
}

/*!
 * \brief  Converts provided value to the opposite endiannness
 *
 * \param[in] value  The value
 * \return Swapped value
 */
float nsk_math_swapflt(float value) {
    union {
        uint32_t u32;
        float    flt;
    } conv = {
        .flt = value
    };

    conv.u32 = nsk_math_swap32(conv.u32);
    return conv.flt;
}
