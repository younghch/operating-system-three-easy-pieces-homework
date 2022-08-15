#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../common_socket.h"
#include "../common_threads.h"

struct  sockaddr_in server_address;

void    *connect_and_send_message(void *message)
{
    int                 client_socket;
    
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    assert(client_socket != -1);
    Connect(client_socket, &server_address, sizeof(server_address));
    Send(client_socket, message, strlen((char*)message), 0);
    close(client_socket);
    return NULL;
}

int     main(int argc, char const *argv[])
{
    int                 num_clients, i;
    int                 client_socket;
    char                buffer[BUFFER_SIZE] = { 0 };
    struct  client_info *client_infos;
	pthread_t	        *threads;
    char                **messages;
    char                *base_message;
    char                alpha_id[2];
    
    assert(argc == 2);
    num_clients = atoi(argv[1]);
    threads = malloc(sizeof(pthread_t)*num_clients);
    messages = malloc(sizeof(char*)*num_clients);
    base_message = "Hello from client : ";

    for (i = 0; i < num_clients; i++)
    {
        messages[i] = malloc(strlen(base_message)+2);
        strlcpy(messages[i], base_message, strlen(base_message)+2);
        messages[i][strlen(base_message)] = i+65;
        messages[i][strlen(base_message)+1] = 0;
    }
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    Inet_pton(AF_INET, LOCAL_HOST, &server_address.sin_addr);

    for (i = 0; i < num_clients; i++)
        Pthread_create(threads+i, NULL, connect_and_send_message, messages[i]);
    for (i = 0; i < num_clients; i++)
        Pthread_join(threads[i], NULL);

    close(client_socket);
    free(threads);
    for (i = 0; i < num_clients; i++)
        free(messages[i]);
    free(messages);
    return 0;

}
