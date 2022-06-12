#include "measure-time.h"

void            start_timer()
{
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
}

void            end_timer()
{
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
}

double          get_elapsed_seconds()
{
    return end.tv_sec + end.tv_nsec / 1000000.0 - start.tv_sec - start.tv_nsec / 1000000.0;
}

unsigned long   get_elapsed_nano_seconds()
{
    return end.tv_sec*1000000 + end.tv_nsec - start.tv_sec*1000000 - start.tv_nsec;
}

