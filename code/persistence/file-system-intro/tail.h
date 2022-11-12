#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void    parse_and_set_args(int argc, char *argv[], int *lines, char **path);

void    read_blocks_backward(struct stat *st, int fd, int lines, char **result);

int     check_lines_in_block(int fd, char *buffer, size_t blk_size);

void    move_pointer_to_start(char** block, int read_start);

void    print_result(char **result);

void    free_all(char **result);
