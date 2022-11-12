#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define FALSE               0
#define TRUE                1

#define FOLDER_LEVEL_PREFIX "=======|"
#define FILE_LEVEL_PREFIX   "--------"
#define PATH_SEPARATOR      '/'

void    print_folder_and_subfiles(char* folder, char level);
void    print_name_with_prefix(char *name, char level, char is_folder);
