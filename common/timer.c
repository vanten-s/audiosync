#include "timer.h"
#include <stddef.h>
#include <sys/time.h>

uint64_t current_time() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return time.tv_usec + 1000000 * time.tv_sec;
}
