#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) 
{
    printf("hello world (pid: %d)\n", (int) getpid());
    int rc = fork();
    if (rc < 0) 
    {
        fprintf(stderr, "fork failled\n");
        exit(1);
    }
    else if (rc == 0)
    {
        close(STDOUT_FILENO);
        open("./results.txt", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
        printf("hello, I am child (pid: %d)\n", (int) getpid());
        char *myargs[3];
        myargs[0] = strdup("wc");
        myargs[1] = strdup("fork.c");
        myargs[2] = NULL;
        execvp(myargs[0], myargs);
        printf("this shouldn't print out");
    }
    else
    {
        int rc_wait = wait(NULL);
        printf("hello, I am parent of %d (pid: %d)\n", rc, (int) getpid());
    }
    return 0;
}