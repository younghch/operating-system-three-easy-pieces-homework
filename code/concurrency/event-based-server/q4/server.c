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

void set_aio_cb(struct aiocb *cb, int fd, int offset)
{
    cb->aio_fildes = fd;
    cb->aio_offset = offset;
    cb->aio_buf = calloc(BUFFER_SIZE, sizeof(char));
    cb->aio_nbytes = BUFFER_SIZE;
}

int main(int argc, char const *argv[])
{
    int                     opt, status;
    int                     master_socket, new_socket, cur_socket, max_sd, cur_file;
    struct  aiocb           aio_cbs[MAX_CLIENTS];
    int                     clients[MAX_CLIENTS] = {0};
    struct  aiocb           *new_task, *cur_task;
    int                     offset;
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
    for (i=0; i < MAX_CLIENTS; i++)
        bzero(&aio_cbs[i], sizeof(struct aiocb));
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
        new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket != -1) 
        {
            printf("=========New connection=======\nsocket fd : %d, ip : %s, port : %d\n",
                    new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
            for (i=0; i < MAX_CLIENTS; i++)
            {
                if (aio_cbs[i].aio_fildes == 0)
                {
                    new_task = &aio_cbs[i];
                    break;
                }   
            }
            set_aio_cb(new_task, new_socket, 0);
            aio_read(new_task);
        }
        for (i=0; i<MAX_CLIENTS; i++)
        {
            cur_task = &aio_cbs[i];
            if (cur_task->aio_fildes == 0)
                continue;
            status = aio_error(cur_task);
            if (status == -1)
            {
                printf("error ocuured!: %d\n", errno);
                continue;
            }
            if (status == EINPROGRESS)
            {
                printf("EINPROGRESS\n");
                continue;
            }
            if (clients[i] == 0)
            {
                printf("open file from server : %s", cur_task->aio_buf);
                cur_socket = cur_task->aio_fildes;
                cur_file = open((char *)cur_task->aio_buf, O_RDONLY);
                aio_return(cur_task);
                if (cur_file == 0 || cur_file == 1){
                    printf("no such file!!\n");
                    cur_task->aio_buf = 0;
                    Send(cur_socket, EOFS, strlen(EOFS), 0);
                } else {
                    set_aio_cb(cur_task, cur_file, 0);
                    aio_write(cur_task);
                    clients[i] = cur_socket;
                }
            }
            else
            {
                char *temp = cur_task->aio_buf; 
                if (temp[0] == 0){
                    Send(clients[i], EOFS, strlen(EOFS), 0);
                    clients[i] = 0;
                    cur_task->aio_buf = 0;
                } else {
                    Send(clients[i], cur_task->aio_buf, strlen(cur_task->aio_buf), 0);
                    
                    cur_file = cur_task->aio_fildes;
                    offset = cur_task->aio_offset + BUFFER_SIZE;
                    aio_return(cur_task);

                    set_aio_cb(cur_task, cur_file, offset);
                    aio_write(cur_task);
                }
            }
        }
    }
}
