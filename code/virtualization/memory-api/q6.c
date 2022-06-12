#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) 
{
    int *data;
    
    data = malloc(100);
    data[10] = 42;
    free(data);
    printf("value inside the pointer : %d\n", data[10]);
    return 0;
}
