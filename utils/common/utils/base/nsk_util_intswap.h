#pragma once

#include <stdint.h>

/*!
 * \brief  Swaps the u64 integer to the other endianness
 *
 * \param[in] val  The value
 * \return Swapped value
 */
static inline __attribute__((always_inline)) uint64_t nsk_swap_uint64(
    uint64_t val
) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap64(val);
#else
    return
        ((val & 0x00000000000000ffull) << 56) |
        ((val & 0x000000000000ff00ull) << 40) |
        ((val & 0x0000000000ff0000ull) << 24) |
        ((val & 0x00000000ff000000ull) << 8)  |
        ((val & 0x000000ff00000000ull) >> 8)  |
        ((val & 0x0000ff0000000000ull) >> 24) |
        ((val & 0x00ff000000000000ull) >> 40) |
        ((val & 0xff00000000000000ull) >> 56);
#endif
}

/*!
 * \brief  Swaps the u32 integer to the other endianness
 *
 * \param[in] val  The value
 * \return Swapped value
 */
static inline __attribute__((always_inline)) uint32_t nsk_swap_uint32(
    uint32_t val
) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap32(val);
#else
    return
        ((val >> 24) & 0x000000ffu) |
        ((val >> 8)  & 0x0000ff00u) |
        ((val << 8)  & 0x00ff0000u) |
        ((val << 24) & 0xff000000u);
#endif
}

/*!
 * \brief  Swaps the u16 integer to the other endianness
 *
 * \param[in] val  The value
 * \return Swapped value
 */
static inline __attribute__((always_inline)) uint16_t nsk_swap_uint16(
    uint16_t val
) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap16(val);
#else
    return
        ((val << 8)  & 0xff00u) |
        ((val >> 8)  & 0x00ffu);
#endif
}

/*!
 * \brief  Swaps the i16 integer to the other endianness
 *
 * \param[in] val  The value
 * \return Swapped value
 */
static inline __attribute__((always_inline)) int16_t nsk_swap_int16(
    int16_t val
) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap16(val);
#else
    return
        ((val << 8)  & 0xff00u) |
        ((val >> 8)  & 0x00ffu);
#endif
}

/*!
 * \brief  Swaps the i32 integer to the other endianness
 *
 * \param[in] val  The value
 * \return Swapped value
 */
static inline __attribute__((always_inline)) int32_t nsk_swap_int32(
    int32_t val
) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap32((uint32_t)val);
#else
    return
        (((uint32_t)val >> 24) & 0x000000ffu) |
        (((uint32_t)val >> 8)  & 0x0000ff00u) |
        (((uint32_t)val << 8)  & 0x00ff0000u) |
        (((uint32_t)val << 24) & 0xff000000u);
#endif
}
