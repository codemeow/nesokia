#ifndef NSK_MATH_ENDIANNESS
#define NSK_MATH_ENDIANNESS

#include <stdint.h>

/*!
 * \brief  Converts provided value to u32-BE
 *
 * \note Takes into account the current endianness
 *
 * \param[in] value  The value
 * \return Value in u32-BE
 */
uint32_t nsk_math_tobe32(uint32_t value);

/*!
 * \brief  Converts provided value to u16-BE
 *
 * \note Takes into account the current endianness
 *
 * \param[in] value  The value
 * \return Value in u16-BE
 */
uint16_t nsk_math_tobe16(uint16_t value);

/*!
 * \brief  Converts provided value to float-BE
 *
 * \note Takes into account the current endianness
 *
 * \param[in] value  The value
 * \return Value in float-BE
 */
float nsk_math_tobeflt(float value);

/*!
 * \brief  Converts provided value to the opposite endiannness
 *
 * \param[in] value  The value
 * \return Swapped value
 */
uint32_t nsk_math_swap32(uint32_t value);

/*!
 * \brief  Converts provided value to the opposite endiannness
 *
 * \param[in] value  The value
 * \return Swapped value
 */
uint16_t nsk_math_swap16(uint16_t value);

/*!
 * \brief  Converts provided value to the opposite endiannness
 *
 * \param[in] value  The value
 * \return Swapped value
 */
float nsk_math_swapflt(float value);

#endif