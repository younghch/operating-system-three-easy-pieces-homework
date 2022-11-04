#include "list-files.h"
#include <errno.h>

int g_total_blocks = 0;

int     main(int argc, char *argv[]) {
    char    *path;
    DIR     *dir;
    char    is_l;

    assert(argc < 4);

    is_l = 0;
    path = ".";
    
    for (int i=1; i < argc; i++){
        if (strcmp(argv[i], "-l") == 0)
            is_l = TRUE;
        else
            path = argv[i];
    }

    dir = opendir(path);
    assert(dir != NULL);

    if (is_l == TRUE)
        ls_l(dir, path);
    else
        ls(dir);

    closedir(dir);

}

void    print_ls(char *name)
{
    if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
        return;

    fputs(name, stdout);
    fputs("\t", stdout);
}

void    print_ls_l(char *full_path, size_t start_idx, char *name)
{
    struct  stat    st;
    struct  passwd  *user;
    struct  group   *group;
    char            permission[11];
    char            time_modify[12];

    if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
        return;
    
    strcpy(full_path + start_idx, name);
    assert(stat(full_path, &st) == 0);
    g_total_blocks += st.st_blocks;
    user = getpwuid(st.st_uid);
    group = getgrgid(st.st_gid);

    set_permission(st.st_mode, permission);
    set_time_format(&st.st_mtimespec, time_modify);
    
    printf("%s\t%hu\t%s\t%s\t%5lld\t%s\t%s\n", permission, st.st_nlink, user->pw_name, group->gr_name, st.st_size, time_modify, name);
}

void    ls(DIR *dir) 
{
    struct dirent   *entry;

    while ((entry = readdir(dir)) != NULL)
        print_ls(entry->d_name);
    puts("");
}

void    ls_l(DIR *dir, char *path) 
{
    struct dirent   *entry;
    char            *full_path;
    size_t          parent_len;

    parent_len = strlen(path);
    full_path = malloc(parent_len+1+MAXNAMLEN+1);
    strcpy(full_path, path);
    full_path[parent_len] = '/';
    
    while ((entry = readdir(dir)) != NULL)
        print_ls_l(full_path, parent_len+1, entry->d_name);
    printf("total %d\n", g_total_blocks);
}

void    set_permission(mode_t mode, char *permission)
{
    for (int i=0; i<10; i++)
        permission[i] = '-';
    permission[10] = 0;

    if (mode & S_IFDIR) permission[0] = 'd';

    if (mode & S_IRUSR) permission[1] = 'r';
    if (mode & S_IWUSR) permission[2] = 'w';
    if (mode & S_IXUSR) permission[3] = 'x';

    if (mode & S_IRGRP) permission[4] = 'r';
    if (mode & S_IWGRP) permission[5] = 'w';
    if (mode & S_IXGRP) permission[6] = 'x';

    if (mode & S_IROTH) permission[7] = 'r';
    if (mode & S_IWOTH) permission[8] = 'w';
    if (mode & S_IXOTH) permission[9] = 'x';
    
}

void    set_time_format(struct timespec *ts, char  *time_format) 
{
    struct  tm  tm;

    gmtime_r(&ts->tv_sec, &tm);
    strftime(time_format, 11, "%m %d %H:%M", &tm);
    time_format[11] = 0;
}