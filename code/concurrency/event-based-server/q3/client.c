#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../common_socket.h"

int main(int argc, char const *argv[])
{
    int                 client_socket;
    int                 num_read;
    struct  sockaddr_in server_address;
    char                buffer[BUFFER_SIZE] = { 0 };
    const   char        *path;

    assert(argc == 2);
    path = argv[1];
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    assert(client_socket != -1);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    Inet_pton(AF_INET, LOCAL_HOST, &server_address.sin_addr);

    Connect(client_socket, &server_address, sizeof(server_address));

    printf("request file from server : %s\n", path);
    Send(client_socket, path, strlen(path), 0);

    puts("read file from server");
    num_read = read(client_socket, buffer, BUFFER_SIZE);
    while(num_read > 0)
    {
        printf("%s", buffer);
        num_read = read(client_socket, buffer, BUFFER_SIZE);
    }
    close(client_socket);
    return 0;

}
