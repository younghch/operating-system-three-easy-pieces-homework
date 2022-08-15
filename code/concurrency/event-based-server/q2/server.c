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
    int                     master_socket, new_socket, sd, max_sd;
    int                     client_socket[MAX_CLIENTS] = { 0 };
    int                     num_ready, valread;
    int                     i, addrlen;
    struct  sockaddr_in     address;
    fd_set                  readfds;
    char                    buffer[BUFFER_SIZE];
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
            sd = client_socket[i];
            if (FD_ISSET(sd, &readfds))
            {
                valread = read(sd, buffer, BUFFER_SIZE-1);
                if (valread == 0)
                {
                    getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                    printf("=========Client disconnected=========\nsip : %s, port: %d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
                    
                    close(sd);
                    client_socket[i] = 0;
                }

                else
                {
                    buffer[valread] = '\0';
                    printf("=========Client message=======\n%s\n", buffer);
                }
            }
        }
    }





}
