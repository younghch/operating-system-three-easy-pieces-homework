#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#define TRUE    1
#define FALSE   0

void    print_ls_stat(char *sub_path);
void    print_ls_l_stat(char *sub_path);
void    ls(DIR *dir);
void    ls_l(DIR *dir);
void    set_permission(mode_t mode, char *permission);
void    set_time_format(struct timespec *ts, char *time_format);