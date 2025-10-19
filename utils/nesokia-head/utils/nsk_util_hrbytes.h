#ifndef NSK_UTIL_HRBYTES
#define NSK_UTIL_HRBYTES

#include <stdint.h>

/*!
 * \brief  Converts bytes into human-readable format (KB/MB/GB + raw value)
 *
 * \param[in] value  The value
 * \return Static string
 */
const char *nsk_util_hrbytes(uint64_t value);

#endif