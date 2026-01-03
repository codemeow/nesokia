#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <nsk_util_meta.h>

#include "../scan/nsk_scan_directory.h"
#include "../arguments/nsk_args_options.h"
#include "../scan/nsk_scan_entry.h"

/*!
 * \brief  Constructs full path to the entry
 *
 * \param[out] out  Constructed path
 * \param[in] path  Directory name
 * \param[in] name  File name
 * \return True on success
 */
static bool _entry_path(
    char        out[static PATH_MAX],
    const char *path,
    const char *name
) {
    int ret = snprintf(
        out,
        PATH_MAX,
        "%s%c%s",
        path,
        nsk_path_delimeter,
        name
    );

    if (ret < 0 || ret >= PATH_MAX) {
        return false;
    }

    return true;
}

/*!
 * \brief  Scans the provided directory and processes every found entry
 *
 * \param[in] directory  The directory
 * \param[in] level      The deepness level (starts with 0)
 */
void nsk_scan_directory(const char *directory, unsigned level) {
    if (!nsk_options_program.recursive && level > 0) {
        return;
    }

    DIR *dir = nsk_io_opendir(directory);
    if (!dir) {
        nsk_err("Error: cannot open dir: \"%s\"\n", directory);
        return;
    }

    const struct dirent *entry;
    while ((entry = nsk_io_readdir(dir))) {
        if (
            strcmp(entry->d_name, "." ) == 0 ||
            strcmp(entry->d_name, "..") == 0
        ) {
            continue;
        }

        char path[PATH_MAX];
        if (!_entry_path(path, directory, entry->d_name)) {
            nsk_err(
                "Error: cannot reach entry: \"%s/%s\"\n",
                directory,
                entry->d_name
            );
            continue;
        }

        nsk_scan_entry(path, level + 1);
    }


    if (closedir(dir)) {
        nsk_err("Error: cannot close dir: \"%s\"\n", directory);
        return;
    }
}
