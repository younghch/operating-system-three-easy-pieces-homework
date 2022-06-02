#include <stdlib.h>

int main(int argc, char *argv[]) 
{
    int *null_pointer;

    null_pointer = NULL;
    free(null_pointer);
}