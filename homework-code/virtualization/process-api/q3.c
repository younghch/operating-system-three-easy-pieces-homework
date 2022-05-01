#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) 
{
    int rc = fork();
    if (rc < 0) 
    {
        fprintf(stderr, "fork failled\n");
        exit(1);
    }
    else if (rc == 0)
    {   
        printf("Hello\n");
    }
    else
    {
        wait(NULL);
        printf("Good bye\n");
    }
    return 0;
}