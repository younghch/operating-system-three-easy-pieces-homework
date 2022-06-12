#include <time.h>

struct  timespec    start, end;

void            start_timer();
void            end_timer();
double          get_elapsed_seconds();
unsigned long   get_elapsed_nano_seconds();
