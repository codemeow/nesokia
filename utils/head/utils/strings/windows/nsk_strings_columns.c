#if defined(NSK_OS_WINDOWS)

#include <string.h>

#include "../../../utils/strings/nsk_strings_columns.h"
#include "../../../utils/nsk_util_min.h"

/*!
 * \brief  Counts bytes of the UTF-8 string that fit into \p requested columns
 *
 * \param[in]  string     The string
 * \param[in]  requested  The requested number of columns
 * \param[out] occupied   The number f columns actually occupied
 * \return  The number of bytes that fit
 */
size_t nsk_strings_limitcolumns(
    const char *string,
    size_t      requested,
    size_t     *occupied
) {
    /* There's no `wcwidth` function under mingw, thus unicode is a bit broken */
    size_t len    = strlen(string);
    size_t result = NSK_MIN(requested, len);
    *occupied     = len;

    return result;
}

/*!
 * \brief  Returns the number of the displayed columns when printing this string
 *
 * \param[in] string  The string
 * \return Number of columns required
 */
size_t nsk_strings_colums(
    const char *string
) {
    /* There's no `wcwidth` function under mingw */
    return strlen(string);
}

#endif