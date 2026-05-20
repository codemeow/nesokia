#include <stdio.h>
#include <string.h>

#include "io/nsk_io_fullpath.h"
#include "io/nsk_io_dirname.h"
#include "io/nsk_io_path.h"
#include "base/nsk_util_cleanup.h"
#include "base/nsk_util_malloc.h"

/*!
 * \brief Program name passed to as argv[0]
 */
const char *nsk_args_program = NULL;

/*!
 * \brief  Resolves relative path to the full path starting with the current
 * binary position
 *
 * \param[in] path  The relative path
 * \return Allocated string with full resolved path
 */
char *nsk_io_fullpath(const char *path) {
    nsk_auto_free char *dir = nsk_io_dirname(nsk_args_program);
    const size_t size = strlen(dir) + 1 + strlen(path) + 1;
    char *string = nsk_util_malloc(size);

    snprintf(
        string,
        size,
        "%s%c%s",
        dir,
        nsk_path_delimeter,
        path
    );

    return string;
}
