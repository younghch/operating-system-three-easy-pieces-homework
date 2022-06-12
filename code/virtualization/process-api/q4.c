#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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
        // execl("/bin/ls", "ls", NULL);
        // execle("/bin/ls", "ls", NULL, NULL);
        // execlp("ls", "ls", NULL);
    }
    else
    {
        char *v_args[2];
        v_args[0] = strdup("ls");
        v_args[1] = NULL;
        // execv("/bin/ls", v_args);
        // execvp("ls", v_args);
        // execvpe("/bin/ls", v_args, NULL);
    }
    return 0;
}