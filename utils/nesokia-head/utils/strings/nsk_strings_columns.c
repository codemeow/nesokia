#define _XOPEN_SOURCE /* wcwidth */

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "../../utils/strings/nsk_strings_columns.h"

#include "../../utils/nsk_util_cleanup.h"
#include "../../utils/nsk_util_malloc.h"

/*!
 * \brief  Module initializer
 */
__attribute__((constructor))
static void _init(void) {
    setlocale(LC_CTYPE, "");
}

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
    size_t  left  = requested;
    size_t  bytes = 0;
    wchar_t wc;

    /* Loop by bytes, counting how many bytes fit into columns */
    while (*string && requested > 0) {
        mbstate_t state = { 0 };
        size_t used = mbrtowc(&wc, string, MB_CUR_MAX, &state);

        if (used == (size_t)-1) {
            /* Invalid byte */
            state = (mbstate_t){ 0 };
            used = 1;
            wc = L'?';

        } else if (used == (size_t)-2) {
            /* Incomplete sequence */
            break;

        } else if (used == 0) {
            /* '\0' */
            used = 1;
            wc = L'\0';
        }

        int width = wcwidth(wc);
        if (width < 0) {
            /* Non-printable */
            width = 0;
        }

        if ((size_t)width > left) {
            /* Won't fit */
            break;
        }

        left      -= (size_t)width;
        string    += used;
        bytes     += used;
    }

    /* What if there are some 0-length characters that still could fit? */
    while (*string) {
        mbstate_t state = { 0 };
        size_t used = mbrtowc(&wc, string, MB_CUR_MAX, &state);
        if (
            used == (size_t)-1 ||
            used == (size_t)-2 ||
            used == 0
        ) {
            break;
        }
        if (wcwidth(wc) > 0) {
            break;
        }
        string += used;
        bytes  += used;
    }

    if (occupied) {
        *occupied = requested - left;
    }
    return bytes;
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
    mbstate_t state = {0};
    wchar_t wc;
    size_t len = 0;
    const char *p = string;

    while (*p) {
        size_t n = mbrtowc(&wc, p, MB_CUR_MAX, &state);
        if (
            n == (size_t)-1 ||
            n == (size_t)-2
        ) {
            break;
        }

        len++;
        p += n;
    }

    nsk_auto_free wchar_t *wbuf = nsk_util_malloc((len + 1) * sizeof(wchar_t));
    if (mbstowcs(wbuf, string, len + 1) == (size_t)-1) {
        return 0;
    }

    int width = wcswidth(wbuf, len);
    if (width < 0) {
        return 0;
    } else {
        return width;
    }
}