#ifndef NSK_STRINGS_COLUMNS
#define NSK_STRINGS_COLUMNS

#include <stddef.h>

/*!
 * \brief  Counts bytes of the UTF-8 string that fit into \p requested columns
 *
 * \param[in]  string     The string
 * \param[in]  requested  The requested number of columns
 * \param[out] occupied   The number f columns actually occupied
 * \return  The number of bytes that fit
 */
size_t nsk_strings_columns(
    const char *string,
    size_t      requested,
    size_t     *used
);

#endif