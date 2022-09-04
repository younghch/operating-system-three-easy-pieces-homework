#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <assert.h>
#include <aio.h>
#include <fcntl.h>
#include "../common_socket.h"

#define MAX_CLIENTS  30

void print_and_continue_read(struct aiocb *task)
{
    printf("=========Client reqeusted file=======\n%s\n", (char *)task->aio_buf);
}

int main(int argc, char const *argv[])
{
    int                     opt, status;
    int                     master_socket, new_socket, client_sock, max_sd;
    struct  aiocb           clients_cb[MAX_CLIENTS];
    struct  aiocb           *new_task, *cur_task;
    int                     num_ready, valread;
    int                     i, addrlen;
    FILE*                   file;
    size_t                  len_line;
    char                    *line;
    struct  sockaddr_in     address;
    fd_set                  readfds;
    char                    *message;

    opt = 1;
    line = NULL;
    message = "No such file!\n";
    master_socket = socket(AF_INET, SOCK_STREAM, 0);
    status = fcntl(master_socket, F_SETFL, fcntl(master_socket, F_GETFL, 0) | O_NONBLOCK);
    for (i=0; i < MAX_CLIENTS; i++)
        bzero(&clients_cb[i], sizeof(struct aiocb));
    assert(master_socket != -1 && status != -1);

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
        new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket != -1) 
        {
            printf("=========New connection=======\nsocket fd : %d, ip : %s, port : %d\n",
                    new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
            for (i=0; i < MAX_CLIENTS; i++)
            {
                if (clients_cb[i].aio_fildes == 0)
                {
                    new_task = &clients_cb[i];
                    break;
                }   
            }
            new_task->aio_fildes = new_socket;
            new_task->aio_offset = 0;
            // new_task->aio_reqprio = 0;
            new_task->aio_buf = calloc(BUFFER_SIZE, sizeof(char));
            new_task->aio_nbytes = BUFFER_SIZE;
            // new_task->aio_sigevent.sigev_notify = SIGEV_NONE;
            aio_read(new_task);
        }
        // READ file 기능 추가, write 추가
        for (i=0; i<MAX_CLIENTS; i++)
        {
            cur_task = &clients_cb[i];
            if (cur_task->aio_fildes == 0)
                continue;
            status = aio_error(cur_task);
            if (status == -1)
            {
                puts("sig -1!!");
                // aio_return(cur_task);
                continue;
            }
            if (status == EINPROGRESS)
                continue;
            printf("not in progress : %d\n", cur_task->aio_fildes);
            print_and_continue_read(cur_task);
        }
    }





}
