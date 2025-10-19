#ifndef NSK_STRINGS_WIDE
#define NSK_STRINGS_WIDE

#if defined(NSK_OS_WINDOWS)

#include <windows.h>

/*!
 * \brief  Converts provided string to Windows Wide String using the provided
 * encoding
 *
 * \param[in] str    The string
 * \param[in] enc    The encoding
 * \param[in] flags  MultiByteToWideChar flags
 * \return Allocated wide string
 */
LPWSTR nsk_string_a2w_enc(const char *str, UINT enc, DWORD flags);

/*!
 * \brief  Converts provided Windows Wide String to string using the provided
 * encoding
 *
 * \param[in] wstr   The wide string
 * \param[in] enc    The encoding
 * \param[in] flags  WideCharToMultiByte flags
 * \return  Allocated string
 */
char *nsk_string_w2a_enc(LPCWSTR wstr, UINT enc, DWORD flags);

/*!
 * \brief  Converts string into Windows Wide String
 *
 * \param[in] str  The string
 * \return Allocated Windows Wide String
 */
LPWSTR nsk_string_a2w(const char *str);

/*!
 * \brief  Converts Windows Wide String to string
 *
 * \param[in] wstr  The Windows Wide String
 * \return Allocated string
 */
char *nsk_string_w2a(LPCWSTR wstr);

#endif

#endif