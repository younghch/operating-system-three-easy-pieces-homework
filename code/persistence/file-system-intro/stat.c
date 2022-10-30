#include <sys/stat.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

void    set_permission(mode_t mode, char *permission){
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

void    set_time_format(struct timespec *ts, char *time_format) {
    struct  tm  tm;

    gmtime_r(&ts->tv_sec, &tm);
    strftime(time_format, 20, "%Y-%m-%d %H:%M:%S", &tm);
}

int     main(int argc, char *argv[]) {
    
    char            *path;
    char            permission[11];
    char            time_format[20];
    struct  stat    stat_;
    int             code;


    assert(argc == 2);
    path = argv[1];



    code = stat(path, &stat_);
    assert(code == 0);
    set_permission(stat_.st_mode, permission);

    printf("File: \"%s\"\n", path);
    printf("Size: %lld\t", stat_.st_size);
    fputs("FileType: ", stdout);
    if (stat_.st_mode & S_IFREG)
        puts("Regular File");
    else if (stat_.st_mode & S_IFDIR)
        puts("Directory");
    else if (stat_.st_mode & S_IFLNK)
        puts("Symbolic Link");
    else
        puts("ETC");
    printf("Mode: %s\tUid: %u\tGid: %u\n", permission, stat_.st_uid, stat_.st_gid);
    printf("Device: %d\tInode: %llu\tLinks: %hu\n", stat_.st_dev, stat_.st_ino, stat_.st_nlink);

    set_time_format(&stat_.st_atimespec, time_format);
    printf("Access: %s\n", time_format);
    set_time_format(&stat_.st_mtimespec, time_format);
    printf("Modify: %s\n", time_format);
    set_time_format(&stat_.st_ctimespec, time_format);
    printf("Change: %s\n", time_format);
    
}