#include <stddef.h>
#include <string.h>

#include "../strings/nsk_strings_strdup.h"
#include "../utils/nsk_util_malloc.h"
#include "../output/nsk_output_err.h"

/*!
 * \brief  Dupes the string value
 *
 * \param[in] value  The value
 * \return Allocated copied string
 */
char *nsk_strings_strdup(const char *value) {
    if (!value) {
        nsk_err("Error: empty string provided to %s\n", __FUNCTION__);
        abort();
    }

    size_t size = strlen(value) + 1;
    char   *str = nsk_util_malloc(size);

    strncpy(str, value, size - 1);

    return str;
}