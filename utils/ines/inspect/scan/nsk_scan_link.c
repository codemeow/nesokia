#include "../scan/nsk_scan_link.h"
#include "../arguments/nsk_args_options.h"
#include "../scan/nsk_scan_directory.h"
#include "../scan/nsk_scan_file.h"
#include "../utils/io/nsk_io_stat.h"
#include "../utils/log/nsk_log_err.h"

/*!
 * \brief  Scan the symbolic link entry
 *
 * \param[in] linkname  The link filename
 * \param[in] level      Deepness level
 */
bool nsk_scan_link(const char *linkname, unsigned level) {
    if (!nsk_options_program.recursive && level > 0) {
        return false;
    }

    if (!nsk_options_program.follow) {
        return false;
    }

    struct stat stat;
    if (!nsk_io_stat(linkname, &stat)) {
        nsk_err("Error: cannot run `stat` on \"%s\"\n", linkname);
        return false;
    }

    if (S_ISDIR(stat.st_mode)) {
        return nsk_scan_directory(linkname, level);

    } else if (S_ISLNK(stat.st_mode)) {
        return nsk_scan_link(linkname, level);

    } else {
        return nsk_scan_file(linkname, level);
    }
}
