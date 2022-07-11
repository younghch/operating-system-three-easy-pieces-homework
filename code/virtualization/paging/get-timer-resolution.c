#include <time.h>
#include <stdio.h>

int main(void)
{

    struct  timespec    t;
    clock_getres(CLOCK_PROCESS_CPUTIME_ID, &t);
    printf("Reolution of the clock: %ld nsec\n", t.tv_nsec);
}