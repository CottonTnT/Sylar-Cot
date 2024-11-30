#include "sylarweb/util.h"
#include <time.h>
#include <stdint.h>
#include <syscall.h>
#include <unistd.h>

namespace Sylar {

auto getElapsedMS()
    -> uint64_t
{
    struct timespec ts = {0};
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

pid_t getThreadId()
{
    return syscall(SYS_gettid);
}

pid_t getFiberId()
{
    return 777;
}

}