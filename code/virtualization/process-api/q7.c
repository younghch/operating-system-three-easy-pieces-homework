#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

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
        close(STDOUT_FILENO);
        printf("Where would this be written?\n");
    }
    else
    {
        wait(NULL);
        printf("parent pid: %d\n", getpid());
    }
    return 0;
}
