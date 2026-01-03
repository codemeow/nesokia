#include <stdlib.h>

#include "arguments/nsk_args_process.h"
#include "scan/nsk_scan_run.h"
#include "queue/nsk_queue_print.h"

int main(int argc, char *argv[]) {
    nsk_args_process(argc, argv);

    nsk_scan_run();
    nsk_queue_print();

    return EXIT_SUCCESS;
}
