#include <libgen.h>
#include <sys/stat.h>
#include <string.h>

#include "../io/nsk_io_mkdirp.h"
#include "../io/nsk_io_mkdir.h"
#include "../io/nsk_io_testd.h"
#include "../nsk_util_cleanup.h"
#include "../strings/nsk_strings_strdup.h"

/*!
 * \brief  Creates the path including all subdirectories
 *
 * \param[in] path  The path
 * \param[in] mode  The mode
 * \return 0 on success, -1 on error with errno being set
 */
int nsk_io_mkdirp(const char *path, mode_t mode) {
    const mode_t parent_mode = mode | S_IWUSR | S_IXUSR;

    if (nsk_io_testd(path)) {
        return 0;
    }

    nsk_auto_free char *copy = nsk_strings_strdup(path);
    const char *parent = dirname(copy);

    if (strcmp(parent, path) != 0 && nsk_io_mkdirp(parent, parent_mode) != 0) {
        return -1;
    }

    return nsk_io_mkdir(path, mode);
}
