#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <assert.h>
#include "../common_socket.h"

#define PORT         8080
#define MAX_CLIENTS  30
int main(int argc, char const *argv[])
{
    int                     opt;
    int                     master_socket, new_socket, client_sock, max_sd;
    int                     client_socket[MAX_CLIENTS] = { 0 };
    int                     num_ready, valread;
    int                     i, addrlen;
    FILE*                   file;
    size_t                  len_line;
    char                    *line;
    struct  sockaddr_in     address;
    fd_set                  readfds;
    char                    buffer[BUFFER_SIZE];
    char                    *message;

    opt = 1;
    line = NULL;
    message = "No such file!\n";
    master_socket = socket(AF_INET, SOCK_STREAM, 0);
    assert(master_socket != -1);

    Setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    addrlen = sizeof(address);

    Bind(master_socket, (struct sockaddr *)&address, addrlen);
    Listen(master_socket, 3);
    printf("Listening on port %d\nWaiting for connections...\n", PORT);

    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        for (i = 0; i <  MAX_CLIENTS; i++)
        {
            client_sock = client_socket[i];
            if (client_sock > 0)
                FD_SET(client_sock, &readfds);
            if (client_sock > max_sd)
                max_sd = client_sock;
        }

        num_ready = select(max_sd+1, &readfds, NULL, NULL, NULL);

        if (num_ready < 0 && errno != EINTR)
            printf("select error!\n");

        if (FD_ISSET(master_socket, &readfds))
        {
            new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
            assert(new_socket != -1);

            printf("=========New connection=======\nsocket fd : %d, ip : %s, port : %d\n",
                    new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

            for (i = 0; i < MAX_CLIENTS; i++)
            {
                if (client_socket[i] == 0)
                {
                    client_socket[i] = new_socket;
                    printf("New socket added to list as %d\n", i);
                    break;
                }
            }
        }

        for (i = 0; i < MAX_CLIENTS; i++)
        {
            client_sock = client_socket[i];
            if (FD_ISSET(client_sock, &readfds))
            {
                valread = read(client_sock, buffer, BUFFER_SIZE-1);
                if (valread == 0)
                {
                    getpeername(client_sock, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                    printf("=========Client disconnected=========\nip : %s, port: %d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
                    close(client_sock);
                    client_socket[i] = 0;
                }
                else
                {
                    buffer[valread] = '\0';
                    printf("=========Client reqeusted file=======\n%s\n", buffer);
                    file = fopen(buffer, "r");
                    if (file == NULL)
                    {
                        Send(client_sock, message, strlen(message), 0);
                    }
                    else
                    {
                        while(getline(&line, &len_line, file) != -1)
                            Send(client_sock, line, strlen(line), 0);
                    }
                }
            }
        }
    }





}
