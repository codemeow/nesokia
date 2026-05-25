#include <stdlib.h>

#include "arguments/nsk_args_process.h"
#include "process/nsk_process.h"

int main(int argc, char *argv[]) {
    switch (nsk_args_process(argc, argv)) {
        case NSK_ARGS_CONTINUE:
            break;

        case NSK_ARGS_EXIT_SUCCESS:
            return EXIT_SUCCESS;

        case NSK_ARGS_EXIT_FAILURE:
            return EXIT_FAILURE;
    }

    nsk_process();

    return EXIT_SUCCESS;
}
