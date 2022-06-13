#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "measure-time.h"

int main(int argc, char *argv[]) 
{
    long    PAGESIZE;
    int     numpages;
    int     trials;
    long    jump;
    int     *a;

    if (argc != 3)
    {
        fprintf(stderr, "Error: %s\n", "arguments must be 2");
        return 1;
    }

    numpages = atoi(argv[1]);
    trials = atoi(argv[2]);
    PAGESIZE = sysconf(_SC_PAGESIZE);
    jump = PAGESIZE/sizeof(int);
    a = calloc(numpages, PAGESIZE);

    start_timer();

    for (int j = 0; j < trials; j++)
        for (int i = 0; i < numpages*jump; i += jump)
            a[i] += 1;

    end_timer();
    printf("%d,%d,%f\n", numpages, trials, get_elapsed_nano_seconds());
    
    free(a);
    return 0;
}

