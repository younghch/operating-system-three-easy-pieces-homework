#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int     g_result_length = 0;

void    parse_and_set_args(int argc, char *argv[], int *lines, char *path) {
    int     c;

    while (optind < argc) {
        if ((c = getopt(argc, argv, "n:")) != -1) {
            switch (c) {
                case 'n':
                    lines = atoi(optarg);
                    break;
                case '?':
                default:
                    break;
            }
        } else {
            path = argv[optind];        
            optind++;
        }
    }
}

void    read_blocks_backward(struct stat *st, int fd, int lines, char **result) {
    blksize_t   blk_size;
    off_t       offset;
    int         lines_found;
    char        *buffer;

    lines_found = 0;
    blk_size = st->st_blksize;
    offset = (st->st_blocks-1)*blk_size;

    // TODO: check line count condition
    while (lines_found < lines && offset >= 0) {
        buffer = malloc(blk_size);
        lseek(fd, offset, SEEK_SET);

        lines_found += check_lines_in_block(fd, buffer, blk_size);
        
        result[g_result_length] = buffer;
        g_result_length++;
        offset -= blk_size;
    }
    
}

int     check_lines_in_block(int fd, char *buffer, size_t blk_size) {
    int count;

    count = 0;
    read(fd, buffer, blk_size);
    // lines_found를 global로 변경하고 찾은 개수를 체크해 offset까지 변경해주는게 나을까?

    return count;
}

void    move_pointer_to_start(char* block) {

}

void    free_all(char **result) {

}

int     main(int argc, char *argv[]) {
    int             lines;
    struct  stat    st;
    blkcnt_t        block_index_backward;
    char            *path;
    char            **result;

    parse_and_set_args(argc, argv, &lines, path);

    assert(stat(path, &st) == 0);
    assert((result = malloc(sizeof(char *)*st.st_blocks)) != NULL);
    read_blocks_backward();

}

