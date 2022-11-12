#include "tail.h"

int     g_result_length = 0;
int     g_free_back_idx = 0;

int     main(int argc, char *argv[]) {
    int             lines, fd;
    struct  stat    st;
    blkcnt_t        block_index_backward;
    char            *path;
    char            **result;

    parse_and_set_args(argc, argv, &lines, &path);
    assert(stat(path, &st) == 0);
    assert((result = malloc(sizeof(char *)*st.st_blocks)) != NULL);
    assert((fd = open(path, O_RDONLY)) != -1);
    read_blocks_backward(&st, fd, lines, result);
    print_result(result);
    free_all(result);

}

void    parse_and_set_args(int argc, char *argv[], int *lines, char **path) {
    int     c;

    while (optind < argc) {
        if ((c = getopt(argc, argv, "n:")) != -1) {
            switch (c) {
                case 'n':
                    *lines = atoi(optarg);
                    break;
                case '?':
                default:
                    break;
            }
        } else {
            *path = argv[optind];  
            optind++;
        }
    }
}

void    read_blocks_backward(struct stat *st, int fd, int lines, char **result) {
    blksize_t   blk_size;
    off_t       offset;
    int         new_line_chs;
    int         read_count;
    char        *buffer;

    new_line_chs = 0;
    blk_size = st->st_blksize;
    offset = (st->st_blocks-1)*blk_size;

    while (new_line_chs < lines && offset >= 0) {
        buffer = malloc(blk_size);
        lseek(fd, offset, SEEK_SET);

        while ((read_count = read(fd, buffer, blk_size)) == 0)
        {
            offset -= blk_size;
            lseek(fd, offset, SEEK_SET);
        }
        
        if (g_result_length == 0 && buffer[read_count-1] == '\n')
            new_line_chs--;

        new_line_chs += check_lines_in_block(fd, buffer, blk_size);
        result[g_result_length] = buffer;
        g_result_length++;
        offset -= blk_size;
    }

    move_pointer_to_start(&result[g_result_length-1], new_line_chs-lines+1);
    
}

int     check_lines_in_block(int fd, char *buffer, size_t blk_size) {
    int     count;
    int     i, ri;
    char    c;

    count = 0;
    i = 0;

    while ((c = buffer[i]) != 0){
        if (c == '\n') count += 1;
        i++;
    }
    return count;
}

void    move_pointer_to_start(char** block, int read_start) {
    int     count;
    char    c;

    count = 0;

    while (count < read_start && (c = (*block)[g_free_back_idx]) != 0){
        if (c =='\n') count += 1;
        g_free_back_idx++;
    }
    *block += g_free_back_idx;
}

void    print_result(char **result) {
    int i;

    i = g_result_length-1;
    while (i >= 0){
        fputs(result[i], stdout);
        i--;
    }
}

void    free_all(char **result) {
    int i;

    i = g_result_length-1;
    free(result[i]-g_free_back_idx);
    i--;
    
    while (i >= 0){
        free(result[i]);
        i--;
    }
    free(result);
}
