#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int global;

int main(int argc, char *argv[]) 
{
    int rc = fork();
    int local = 0;

    if (rc < 0) 
    {
        fprintf(stderr, "fork failled\n");
        exit(1);
    }
    else if (rc == 0)
    {   
        printf("child process initial local, global value : %d, %d\n", local, global);
        local += 1;
        global = 200;
        printf("child process changed local, global value : %d, %d\n", local, global);
    }
    else
    {
        printf("parent process initial local, global value : %d, %d\n", local, global);
        local = 100;
        global += 1;
        printf("parent process changed local, global value : %d, %d\n", local, global);
    }
    return 0;
}