#ifndef NSK_STRINGS_WIDE
#define NSK_STRINGS_WIDE

#if defined(NSK_OS_WINDOWS)

#include <windows.h>

#include "base/nsk_util_attributes.h"

/*!
 * \brief  Converts provided string to Windows Wide String using the provided
 * encoding
 *
 * \param[in] str    The string
 * \param[in] enc    The encoding
 * \param[in] flags  MultiByteToWideChar flags
 * \return Allocated wide string, or NULL on conversion error
 */
nsk_attr_result_unused
nsk_attr_args_nonnull(1)
LPWSTR nsk_string_a2w_enc(
    const char *str,
    UINT enc,
    DWORD flags
);

/*!
 * \brief  Converts provided Windows Wide String to string using the provided
 * encoding
 *
 * \param[in] wstr   The wide string
 * \param[in] enc    The encoding
 * \param[in] flags  WideCharToMultiByte flags
 * \return  Allocated string, or NULL on conversion error
 */
nsk_attr_result_unused
nsk_attr_args_nonnull(1)
char *nsk_string_w2a_enc(
    LPCWSTR wstr,
    UINT enc,
    DWORD flags
);

/*!
 * \brief  Converts string into Windows Wide String
 *
 * \param[in] str  The string
 * \return Allocated Windows Wide String, or NULL on conversion error
 */
nsk_attr_result_unused
nsk_attr_args_nonnull(1)
LPWSTR nsk_string_a2w(
    const char *str
);

/*!
 * \brief  Converts Windows Wide String to string
 *
 * \param[in] wstr  The Windows Wide String
 * \return Allocated string, or NULL on conversion error
 */
nsk_attr_result_unused
nsk_attr_args_nonnull(1)
char *nsk_string_w2a(
    LPCWSTR wstr
);

#endif

#endif
