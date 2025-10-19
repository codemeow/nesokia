#include <stdlib.h>

#include "arguments/nsk_args_process.h"
#include "scan/nsk_scan_run.h"

int main(int argc, char *argv[]) {
    nsk_args_process(argc, argv);

    nsk_scan_run();

    return EXIT_SUCCESS;
}
