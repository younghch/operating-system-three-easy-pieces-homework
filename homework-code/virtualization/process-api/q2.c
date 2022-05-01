#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) 
{
    int fd = open("./concurrently-edit.txt", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
    int rc = fork();

    if (rc < 0) 
    {
        fprintf(stderr, "fork failled\n");
        exit(1);
    }
    else if (rc == 0)
    {
        write(fd, "child writes second line\n", 26);   
    }
    else
    {
        write(fd, "parent writes first line\n", 26);
        wait(NULL);
        write(fd, "parent writes third line\n", 26);
    }
    return 0;
}