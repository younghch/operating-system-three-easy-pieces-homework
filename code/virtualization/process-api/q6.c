#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) 
{
    int status;
    int rc = fork();

    if (rc < 0) 
    {
        fprintf(stderr, "fork failled\n");
        exit(1);
    }
    else if (rc == 0)
    {   
        printf("child pid: %d\n", getpid());
        sleep(3);
        printf("child ended\n");
    }
    else
    {
        printf("parent pid: %d\n", getpid());
        int ret = waitpid(rc, &status, WNOHANG);
        printf("wait pid return: %d\nchild exit normally: %s\nchild exit with code 0: %s\n",
         ret, WIFEXITED(status) ? "true" : "false", WEXITSTATUS(status) == 0 ? "true" : "false");
    }
    return 0;
}
