#include <stdlib.h>

#include "arguments/nsk_args_process.h"
#include "process/nsk_process_input.h"
#include "process/nsk_process_output.h"

int main(int argc, char *argv[]) {
    nsk_args_process(argc, argv);

    nsk_process_input();
    nsk_process_output();

    return EXIT_SUCCESS;
}
