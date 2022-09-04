#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <assert.h>
#include <aio.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main(int argc, char const *argv[])
{
    const   char    *path;
    struct  aiocb   *cb, *new_cb;
    int             fd;
    int             status;

    assert(argc == 2);
    path = argv[1];

    fd = open(path, O_RDONLY);
    assert(fd != -1);
    
    cb = calloc(1, sizeof(struct aiocb));
    cb->aio_fildes = fd;
    cb->aio_buf = calloc(1024, sizeof(char));
    cb->aio_offset = 0;
    cb->aio_nbytes = BUFFER_SIZE;
    
    printf("%d\n", cb->aio_fildes);
    aio_read(cb);
    while (1)
    {
        status = aio_error(cb);
        if (status == 0)
        {
            if (((char *)cb->aio_buf)[0] == 0)
            {
                printf("finished reading\n");
                break;
            }
            printf("%s", (char *)cb->aio_buf);

            ((char*)cb->aio_buf)[0] = 0;
            new_cb = calloc(1, sizeof(struct aiocb));
            new_cb->aio_fildes = cb->aio_fildes;
            new_cb->aio_buf = cb->aio_buf;
            new_cb->aio_offset = cb->aio_offset + BUFFER_SIZE;
            new_cb->aio_nbytes = BUFFER_SIZE;
            aio_return(cb);
            free(cb);
            cb = new_cb;
            aio_read(cb);
        }
    }
}