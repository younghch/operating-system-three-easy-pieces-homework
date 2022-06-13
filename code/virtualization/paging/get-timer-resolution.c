#include <stdio.h>
#include "measure-time.h"

int main(void)
{
    start_timer();
    end_timer();
    printf("elapsed time in seconds: %f\nelapsed time in nano seconds: %f\n", get_elapsed_seconds(), get_elapsed_nano_seconds());
}