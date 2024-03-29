#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "common_socket.h"

int main(int argc, char const *argv[])
{
    int                 server_socket, new_socket;
    int                 opt, addrlen;
    struct  sockaddr_in address;
    char                *hello;
    char                buffer[BUFFER_SIZE] = { 0 };

    hello = "Hello from server";

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    assert(server_socket != -1);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    Bind(server_socket, &address, sizeof(address));
    Listen(server_socket, 3);

    new_socket = accept(server_socket, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    assert(new_socket != -1);
    
    read(new_socket, buffer, BUFFER_SIZE);
    printf("========Server=======\nread from client : %s\n", buffer);
    Send(new_socket, hello, strlen(hello), 0);
    printf("========Server=======\nmessage sent to client\n");

    close(new_socket);
    shutdown(server_socket, SHUT_RDWR);
    return 0;
}

