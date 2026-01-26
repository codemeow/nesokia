#include <threads.h>
#include <limits.h>
#include <stdio.h>

#include "../io/nsk_io_fullpath.h"
#include "../io/nsk_io_dirname.h"
#include "../io/nsk_io_path.h"

/*!
 * \brief  Number of static strings in functions, returning static strings
 */
#define _STATIC_CAROUSEL_SIZE (5)

/*!
 * \brief Program name passed to as argv[0]
 */
const char *nsk_args_program = NULL;

/*!
 * \brief  Resolves relative path to the full path starting with the current
 * binary position
 *
 * \param[in] path  The relative path
 * \return Static string with full resolved path
 */
const char *nsk_io_fullpath(const char *path) {
    static thread_local char string[_STATIC_CAROUSEL_SIZE][PATH_MAX];
    static thread_local size_t index;

    if (++index >= _STATIC_CAROUSEL_SIZE) {
        index = 0;
    }

    snprintf(
        string[index],
        sizeof(string[index]),
        "%s%c%s",
        nsk_io_dirname(nsk_args_program),
        nsk_path_delimeter,
        path
    );

    return string[index];
}