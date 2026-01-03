#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include "../io/nsk_io_dirname.h"
#include "../nsk_util_cleanup.h"
#include "../strings/nsk_strings_strdup.h"

/*!
 * \brief  Number of static strings in functions, returning static strings
 */
#define _STATIC_CAROUSEL_SIZE (5)

/*!
 * \brief  Returns the directory path of the provided filename
 *
 * \param[in] filename  The filename
 * \return Static string
 */
const char *nsk_io_dirname(const char *filename) {
    static char string[_STATIC_CAROUSEL_SIZE][PATH_MAX];
    static size_t index;

    if (++index >= _STATIC_CAROUSEL_SIZE) {
        index = 0;
    }

    nsk_auto_free char *copy = nsk_strings_strdup(filename);
    const char *path = dirname(copy);

    snprintf(
        string[index],
        sizeof(string[index]),
        "%s",
        path
    );

    return string[index];
}