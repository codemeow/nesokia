#if defined(NSK_OS_WINDOWS)

#include <stdlib.h>
#include <windows.h>

#include "../../strings/windows/nsk_strings_wide.h"

#include "../../log/nsk_log_err.h"
#include "../../nsk_util_cleanup.h"
#include "../../nsk_util_malloc.h"

/*!
 * \brief  Converts provided string to Windows Wide String using the provided
 * encoding
 *
 * \param[in] str    The string
 * \param[in] enc    The encoding
 * \param[in] flags  MultiByteToWideChar flags
 * \return Allocated wide string
 */
LPWSTR nsk_string_a2w_enc(const char *str, UINT enc, DWORD flags) {
    const int wstr_size = MultiByteToWideChar(enc, flags, str, -1, NULL, 0);
    if (wstr_size == 0) {
        nsk_err("Cannot convert provided string \"%s\" into wide string", str);
        exit(EXIT_FAILURE);
    }

    LPWSTR wstr = nsk_util_malloc(wstr_size * sizeof(wstr[0]));

    if (MultiByteToWideChar(enc, flags, str, -1, wstr, wstr_size) == 0) {
        nsk_util_free(wstr);
        nsk_err("Cannot convert provided string \"%s\" into wide string", str);
        exit(EXIT_FAILURE);
    }

    return wstr;
}

/*!
 * \brief  Converts provided Windows Wide String to string using the provided
 * encoding
 *
 * \param[in] wstr   The wide string
 * \param[in] enc    The encoding
 * \param[in] flags  WideCharToMultiByte flags
 * \return  Allocated string
 */
char *nsk_string_w2a_enc(LPCWSTR wstr, UINT enc, DWORD flags) {
    const int str_size = WideCharToMultiByte(
        enc, flags, wstr, -1, NULL, 0, NULL, NULL
    );
    if (str_size == 0) {
        nsk_err("Cannot convert provided wide string into string");
        exit(EXIT_FAILURE);
    }

    char *str = nsk_util_malloc(str_size);
    int res = WideCharToMultiByte(
        enc, flags, wstr, -1, str, str_size, NULL, NULL
    );

    if (res == 0) {
        nsk_util_free(str);
        nsk_err("Cannot convert provided wide string into string");
        exit(EXIT_FAILURE);
    }

    return str;
}

/*!
 * \brief  Converts string into Windows Wide String
 *
 * \param[in] str  The string
 * \return Allocated Windows Wide String
 */
LPWSTR nsk_string_a2w(const char *str) {
    return nsk_string_a2w_enc(str, CP_UTF8, MB_ERR_INVALID_CHARS);
}

/*!
 * \brief  Converts Windows Wide String to string
 *
 * \param[in] wstr  The Windows Wide String
 * \return Allocated string
 */
char *nsk_string_w2a(LPCWSTR wstr) {
    return nsk_string_w2a_enc(wstr, CP_UTF8, WC_ERR_INVALID_CHARS);
}

#endif
