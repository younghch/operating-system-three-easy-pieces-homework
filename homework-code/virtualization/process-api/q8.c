#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define READ 0
#define WRITE 1

int main(int argc, char *argv[]) 
{
    int fd[2];
    char buffer[256];
    int rc1, rc2;

    if (pipe(fd) < 0){
        fprintf(stderr, "pipe failled\n");
        exit(1);
    }
    if ((rc1 = fork()) < 0 || (rc2 = fork()) < 0) 
    {
        fprintf(stderr, "fork failled\n");
        exit(1);
    }
    if (rc1 == 0)
    {
        close(fd[READ]);
        strcpy(buffer, "hi from child1");
        write(fd[WRITE], buffer, strlen(buffer));
    }
    else if (rc2 == 0)
    {   
        close(fd[WRITE]);
        read(fd[READ], buffer, 256);
        printf("message received in child 2, message : %s\n", buffer);
    }
    
    return 0;
}
