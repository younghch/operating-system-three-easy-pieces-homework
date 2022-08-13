#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include "common_socket.h"

int server_socket;

void    close_socket(int signal)
{
    shutdown(server_socket, SHUT_RDWR);
    printf("\nServer down\n");
    exit(0);
}

int     main(int argc, char const *argv[])
{
    int                 new_socket;
    int                 opt, addrlen;
    struct  sockaddr_in address;
    time_t              current;
    char                *message;
    char                buffer[BUFFER_SIZE] = { 0 };

    signal(SIGINT, close_socket);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    assert(server_socket != -1);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    Bind(server_socket, &address, sizeof(address));
    Listen(server_socket, 3);

    while (1)
    {
        new_socket = accept(server_socket, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        assert(new_socket != -1);
        
        time(&current);
        message = ctime(&current);
        Send(new_socket, message, strlen(message), 0);
        
        close(new_socket);
    }
    
    shutdown(server_socket, SHUT_RDWR);
    return 0;
}

