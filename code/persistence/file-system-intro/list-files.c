#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#define TRUE    1
#define FALSE   0


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

void    set_time_format(struct timespec *ts, char *time_format) 
{
    struct  tm  tm;

    gmtime_r(&ts->tv_sec, &tm);
    strftime(time_format, 12, "%m %d %H:%M", &tm);
}

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
        if (strcmp(argv[1], "l") == 0)
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
        if (strcmp(argv[1], "l") == 0)
            path = argv[2];
        else if (strcmp(argv[2], "l") == 0)
            path = argv[1];
        else
            assert(TRUE == FALSE);
    }

    dir = opendir(path);

    if (ls_l == TRUE)
        ls_l(dir);
    else
        ls(dir);
        
    closedir(dir);

}

void    print_ls_stat(char *sub_path)
{

}

void    print_ls_l_stat(char *sub_path)
{

}

void    ls(DIR *dir) 
{


}

void    ls_l(DIR *dir) 
{

}