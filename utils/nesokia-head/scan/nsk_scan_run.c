#include "../scan/nsk_scan_run.h"
#include "../scan/nsk_scan_entry.h"
#include "../arguments/nsk_args_options.h"

/*!
 * \brief  Sets the filter string for the scan process
 */
void nsk_scan_run(void) {
    char *const *files = nsk_options_program.files;
    while (*files) {
        nsk_scan_entry(*files, 0);
        files++;
    }
}