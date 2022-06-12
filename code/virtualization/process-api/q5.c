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
        printf("child pid: %d\n", getpid());
        int wait_return = wait(NULL);
        printf("child wait return: %d\n", wait_return);
    }
    else
    {
        int wait_return = wait(NULL);
        printf("wait return: %d\n", wait_return);
    }
    return 0;
}