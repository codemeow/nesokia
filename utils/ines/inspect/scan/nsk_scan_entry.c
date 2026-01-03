#include <sys/stat.h>
#include <nsk_util_meta.h>

#include "../scan/nsk_scan_entry.h"
#include "../scan/nsk_scan_directory.h"
#include "../scan/nsk_scan_file.h"
#include "../scan/nsk_scan_link.h"

/*!
 * \brief  Scans the provided entry and either provides detailed description
 * or puts it into table's stack
 *
 * \param[in] entryname  The entry file-/directoryname
 * \param[in] level      Deepness level
 */
void nsk_scan_entry(const char *entryname, unsigned level) {
    struct stat stat;
    if (nsk_io_lstat(entryname, &stat) != 0) {
        nsk_err("Error: cannot `lstat` file: \"%s\"\n", entryname);
        return;
    }

    if (S_ISDIR(stat.st_mode)) {
        nsk_scan_directory(entryname, level);

    } else if (S_ISLNK(stat.st_mode)) {
        nsk_scan_link(entryname, level);

    } else {
        nsk_scan_file(entryname, level);
    }
}