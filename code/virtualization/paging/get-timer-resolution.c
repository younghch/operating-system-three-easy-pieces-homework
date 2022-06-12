#include <stdio.h>
#include "measure-time.h"

int main(void)
{
    start_timer();
    end_timer();
    printf("elapsed time in seconds: %lf\nelapsed time in nano seconds: %lu\n", get_elapsed_seconds(), get_elapsed_nano_seconds());
}