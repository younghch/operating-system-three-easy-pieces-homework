#include "measure-time.h"

void        start_timer()
{
    clock_gettime(CLOCK_REALTIME, &start);
}

void        end_timer()
{
    clock_gettime(CLOCK_REALTIME, &end);
}

float       get_elapsed_seconds()
{
    return end.tv_sec + end.tv_nsec/1E9 - start.tv_sec - start.tv_nsec/1E9;
}

long long   get_elapsed_nano_seconds()
{
    return end.tv_sec*1E9 + end.tv_nsec - start.tv_sec*1E9 - start.tv_nsec;
}


