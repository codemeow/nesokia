#include <time.h>

#include "nsk_util_time.h"

double nsk_time_stamp(void) {
    struct timespec time = {0};

    clock_gettime(CLOCK_MONOTONIC, &time);

    return
        (double)time.tv_sec * 1000.0 +
        (double)time.tv_nsec / 1000000.0;
}
