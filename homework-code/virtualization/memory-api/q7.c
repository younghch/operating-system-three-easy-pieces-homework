#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) 
{
    int *data;
    
    data = malloc(100);
    data[0] = 42;
    data += 5;
    free(data);
}
