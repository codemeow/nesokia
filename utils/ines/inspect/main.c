#include <stdlib.h>
#include <nsk_util_meta.h>

#include "arguments/nsk_args_process.h"
#include "scan/nsk_scan_run.h"
#include "queue/nsk_queue_print.h"

int main(int argc, char *argv[]) {
    nsk_args_process(argc, argv);

    if (!nsk_scan_run()) {
        nsk_err("Error: no valid ROM files found\n");
        return EXIT_FAILURE;
    }

    nsk_queue_print();

    return EXIT_SUCCESS;
}
