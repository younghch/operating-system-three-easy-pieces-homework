#include <time.h>

struct  timespec    start, end;

void        start_timer();
void        end_timer();
float       get_elapsed_seconds();
long long   get_elapsed_nano_seconds();
