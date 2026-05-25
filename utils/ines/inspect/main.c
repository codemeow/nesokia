#include <stdlib.h>
#include <nsk_util_meta.h>

#include "arguments/nsk_args_process.h"
#include "scan/nsk_scan_run.h"
#include "queue/nsk_queue_print.h"

int main(int argc, char *argv[]) {
    switch (nsk_args_process(argc, argv)) {
        case NSK_ARGS_CONTINUE:
            break;

        case NSK_ARGS_EXIT_SUCCESS:
            return EXIT_SUCCESS;

        case NSK_ARGS_EXIT_FAILURE:
            return EXIT_FAILURE;
    }

    if (!nsk_scan_run()) {
        nsk_err("Error: no valid ROM files found\n");
        return EXIT_FAILURE;
    }

    nsk_queue_print();

    return EXIT_SUCCESS;
}
