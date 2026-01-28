#include <stdlib.h>

#include "arguments/nsk_args_process.h"
#include "process/nsk_process.h"

int main(int argc, char *argv[]) {
    nsk_args_process(argc, argv);

    nsk_process();

    return EXIT_SUCCESS;
}
