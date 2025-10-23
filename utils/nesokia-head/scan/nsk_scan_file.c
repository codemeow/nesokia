#include "../scan/nsk_scan_file.h"

#include "../arguments/nsk_args_options.h"
#include "../types/entry/nsk_entry_process.h"

/*!
 * \brief  Scans the provided file and either provides detailed description
 * or puts it into table's stack
 *
 * \param[in] filename  The filename
 * \param[in] level      Deepness level
 */
void nsk_scan_file(const char *filename, unsigned level) {
    if (!nsk_options_program.recursive && level > 0) {
        return;
    }

    nsk_entry_process(filename);
}