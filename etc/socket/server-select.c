#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include "common_socket.h"

#define MAX_CLIENTS  30
int main(int argc, char const *argv[])
{
    int                     opt;
    int                     master_socket, new_socket, sd, max_sd;
    int                     client_socket[MAX_CLIENTS] = { 0 };
    int                     num_of_ready, valread;
    int                     i, addrlen;
    struct  sockaddr_in     address;
    fd_set                  readfds;
    char                    buffer[BUFFER_SIZE+1];
    char                    *message;

    opt = 1;
    message = "Hello from server!\n";
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
            sd = client_socket[i];
            if (sd > 0)
                FD_SET(sd, &readfds);
            if (sd > max_sd)
                max_sd = sd;
        }

        num_of_ready = select(max_sd+1, &readfds, NULL, NULL, NULL);

        if (num_of_ready < 0 && errno != EINTR)
            printf("select error!\n");

        if (FD_ISSET(master_socket, &readfds))
        {
            new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
            assert(new_socket != -1);

            printf("=========New connection=======\nsocket fd : %d, ip : %s, port : %d\n",
                    new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
            Send(new_socket, message, strlen(message), 0);
            puts("Message sent successfully");

            for (i = 0; i < MAX_CLIENTS; i++)
            {
                if (client_socket[i] == 0)
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n", i);
                    break;
                }
            }
        }

        for (i = 0; i < MAX_CLIENTS; i++)
        {
            sd = client_socket[i];
            if (FD_ISSET(sd, &readfds))
            {
                puts("=========Client message=======");
                valread = read(sd, buffer, 1024);
                if (valread == 0)
                {
                    getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                    printf("ip : %s, port: %d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
                    
                    close(sd);
                    client_socket[i] = 0;
                }

                else
                {
                    buffer[valread] = '\0';
                    printf("%s\n", buffer);
                    Send(sd, message, strlen(message), 0);
                }
            }
        }
    }





}
