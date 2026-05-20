#include <libgen.h>
#include <stdio.h>
#include <string.h>

#include "io/nsk_io_dirname.h"
#include "base/nsk_util_cleanup.h"
#include "base/nsk_util_malloc.h"
#include "strings/nsk_strings_strdup.h"

/*!
 * \brief  Returns the directory path of the provided filename
 *
 * \param[in] filename  The filename
 * \return Allocated string
 */
char *nsk_io_dirname(const char *filename) {
    nsk_auto_free char *copy = nsk_strings_strdup(filename);
    const char *path = dirname(copy);
    const size_t size = strlen(path) + 1;
    char *string = nsk_util_malloc(size);

    snprintf(
        string,
        size,
        "%s",
        path
    );

    return string;
}
