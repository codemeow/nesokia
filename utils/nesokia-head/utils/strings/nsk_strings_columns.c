#define _XOPEN_SOURCE /* wcwidth */

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "../../utils/strings/nsk_strings_columns.h"

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
size_t nsk_strings_columns(
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
