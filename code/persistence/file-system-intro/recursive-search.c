#include "recursive-search.h"

int     main(int argc, char *argv[])
{
    char    *folder;

    assert(argc < 3);
    if (argc == 1)
        folder = ".";
    else
        folder = argv[1];
    print_folder_and_subfiles(folder, 0);
    return 0;
}

void    print_folder_and_subfiles(char* folder, char level)
{
    DIR             *cur_open_folder;
    char            *nxt_folder;
    struct dirent   *content;
    char            is_directory;
    int             cur_path_len, nxt_path_len;

    cur_open_folder = opendir(folder);
    cur_path_len = strlen(folder);
    while ((content = readdir(cur_open_folder)) != NULL)
    {
        if (strcmp(content->d_name, ".") == 0 || strcmp(content->d_name, "..") == 0)
            continue;
        is_directory = content->d_type == DT_DIR ? TRUE : FALSE;
        if (is_directory)
        {
            print_name_with_prefix(content->d_name, level+1, TRUE);

            nxt_path_len = strlen(content->d_name);
            assert((nxt_folder = malloc(cur_path_len+1+nxt_path_len+1)) != NULL);
            strcpy(nxt_folder, folder);
            nxt_folder[cur_path_len] = PATH_SEPARATOR;
            strcpy(nxt_folder+cur_path_len+1, content->d_name);

            print_folder_and_subfiles(nxt_folder, level+1);
            free(nxt_folder);
        }
        else
            print_name_with_prefix(content->d_name, level, FALSE);
    }
}

void    print_name_with_prefix(char *name, char level, char is_folder)
{
    int     i;
    char    *prefix;

    i = 0;
    prefix = is_folder ? FOLDER_LEVEL_PREFIX : FILE_LEVEL_PREFIX;
    while (i++ < level)
        fputs(prefix, stdout);
    puts(name);
}