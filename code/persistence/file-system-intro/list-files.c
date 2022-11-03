#include "list-files.h"

int     main(int argc, char *argv[]) {
    char    *path;
    DIR     *dir;
    char    is_l;

    assert(argc < 4);
    is_l = 0;

    if (argc == 1) 
        path = ".";
    else if (argc == 2)
    {
        if (strcmp(argv[1], "-l") == 0)
        {
            is_l = TRUE;
            path = ".";
        } 
        else
            path = argv[1];
    } 
    else 
    {
        is_l = TRUE;
        if (strcmp(argv[1], "-l") == 0)
            path = argv[2];
        else if (strcmp(argv[2], "-l") == 0)
            path = argv[1];
        else
            assert(TRUE == FALSE);
    }

    dir = opendir(path);
    assert(dir != NULL);

    if (is_l == TRUE)
        ls_l(dir);
    else
        ls(dir);

    closedir(dir);

}

void    print_ls(char *sub_path)
{
    if (strcmp(sub_path, ".") == 0 || strcmp(sub_path, "..") == 0)
        return;

    fputs(sub_path, stdout);
    fputs("\t", stdout);
}

void    print_ls_l(char *sub_path)
{
    struct  stat    st;
    struct  passwd  *user;
    struct  group   *group;
    char            permission[10];
    char            time_modify[12];

    if (strcmp(sub_path, ".") == 0 || strcmp(sub_path, "..") == 0)
        return;
    
    assert(stat(sub_path, &st) == 0);

    user = getpwuid(st.st_uid);
    group = getgrgid(st.st_gid);

    set_permission(st.st_mode, permission);
    set_time_format(&st.st_mtimespec, time_modify);
    
    puts(sub_path);
    printf("%s\n", sub_path);
    printf("%s\t%hu\t%s\t%s\t%5lld\t%s\n", permission, st.st_nlink, user->pw_name, group->gr_name, st.st_size, time_modify);
    printf("%s\t%hu\t%s\t%s\t%5lld\t%s\t%s", permission, st.st_nlink, user->pw_name, group->gr_name, st.st_size, time_modify, sub_path);
    
}

void    ls(DIR *dir) 
{
    struct dirent   *entry;

    while ((entry = readdir(dir)) != NULL)
        print_ls(entry->d_name);
    puts("");
}

void    ls_l(DIR *dir) 
{
    struct dirent   *entry;

    while ((entry = readdir(dir)) != NULL)
        print_ls_l(entry->d_name);
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