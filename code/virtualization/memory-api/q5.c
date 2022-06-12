#include <stdlib.h>

int main(int argc, char *argv[]) 
{
    int *data;
    
    data = malloc(100);
    data[100] = 0;
    free(data);
    return 0;
}
