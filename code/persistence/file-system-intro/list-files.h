#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <assert.h>

#define TRUE    1
#define FALSE   0

void    print_ls(char *sub_path);
void    print_ls_l(char *full_path, size_t start_idx, char *name);
void    ls(DIR *dir);
void    ls_l(DIR *dir, char *path);
void    set_permission(mode_t mode, char *permission);
void    set_time_format(struct timespec *ts, char *time_format);