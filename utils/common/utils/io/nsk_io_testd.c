#include "../io/nsk_io_testd.h"
#include "../io/nsk_io_stat.h"

/*!
 * \brief  Tests the directory for existence.
 *
 * \param[in] path  The path
 * \return True if the directory exists and accessible
 */
bool nsk_io_testd(const char *path) {
    struct stat stat;
    if (nsk_io_stat(path, &stat) != 0) {
        return false;
    }

    return S_ISDIR(stat.st_mode);
}