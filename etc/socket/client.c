#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "common_socket.h"

int main(int argc, char const *argv[])
{
    int                 client_socket;
    struct  sockaddr_in server_address;
    char                *hello;
    char                buffer[BUFFER_SIZE] = { 0 };

    hello = "Hello from client";
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    assert(client_socket != -1);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    Inet_pton(AF_INET, LOCAL_HOST, &server_address.sin_addr);

    Connect(client_socket, &server_address, sizeof(server_address));

    Send(client_socket, hello, strlen(hello), 0);
    printf("========Client=======\nmessage sent to server\n");
    read(client_socket, buffer, BUFFER_SIZE);
    printf("========Client=======\nread from server : %s\n", buffer);

    close(client_socket);
    return 0;

}
