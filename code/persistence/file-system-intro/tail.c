#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void    parse_and_set_args(int argc, char *argv[], int *lines_left, char *path) {
    int     c;

    while (optind < argc) {
        if ((c = getopt(argc, argv, "n:")) != -1) {
            switch (c) {
                case 'n':
                    lines_left = atoi(optarg);
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

void    set_block_size(char *path, blkcnt_t* block_count) {
    struct  stat    st;
    assert(stat(path, &st) == 0);
    *block_count = st.st_blocks;
}

void    read_blocks_backward() {

}

void    check_lines_in_block() {

}

int     main(int argc, char *argv[]) {
    int         lines_left;
    blkcnt_t    block_count;
    blkcnt_t    block_index_backward;
    char        *path;
    char        **matched_blocks;


    parse_and_set_args(argc, argv, &lines_left, path);
    set_block_size(path, &block_count);
    read_blocks_backward()

}

