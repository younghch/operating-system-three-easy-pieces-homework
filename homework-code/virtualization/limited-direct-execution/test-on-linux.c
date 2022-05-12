#define _GNU_SOURCE
#define READ    0
#define WRITE   1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sched.h>

double get_elapsed_seconds(struct timeval start, struct timeval end)
{
    return end.tv_sec + end.tv_usec / 1000000.0 - start.tv_sec - start.tv_usec / 1000000.0;
}

int measure_system_call(int number_of_loop)
{
    int count;
    struct  timeval start, end;
    
    count = 0;
    if (gettimeofday(&start, NULL) == -1)
        return 1;
    while (count < number_of_loop){
        read(0, NULL, 0);
        count++;
    }
    if (gettimeofday(&end, NULL)  == -1)
        return 1;
    printf("average time to execute %d syscall: %fs\n", number_of_loop, get_elapsed_seconds(start, end));
    return 0;   
}

int measure_context_swtich(int number_of_loop)
{
    cpu_set_t       set;
    int             count, p1;
    unsigned int    cpu_id;
    int             fd[2];
    struct          timeval start, end;

    CPU_ZERO(&set);
    CPU_SET(0, &set);
    count = 0;
    if 
    (
        pipe(fd) == -1 || 
        (p1 = fork()) == -1 ||
        sched_setaffinity(getpid(), sizeof(cpu_set_t), &set) == -1
    ){
        printf("something wrong here\n");
        return 1;
    }
    printf("process : %d\n", p1);
    if (p1 != 0)
    {
        if (gettimeofday(&start, NULL)  == -1)
            return 1;
        close(fd[READ]);
        while (count < number_of_loop)
        {
            write(fd[WRITE], NULL, 0);
            count += 2;
            write(1, "parent write\n", 14);
        }
        if (gettimeofday(&end, NULL) == -1 || getcpu(&cpu_id, NULL) == -1)
            return 1;
        printf("parent done on cpu %d\n", cpu_id);
        printf("average time to execute %d context swtich: %fs\n", number_of_loop, get_elapsed_seconds(start, end));
    }
    else 
    {
        close(fd[WRITE]);
        while (count < number_of_loop)
        {
            read(fd[READ], NULL, 0);     
            count += 2;
            printf("child read\n");
        }
        if (getcpu(&cpu_id, NULL) == -1)
            return 1;
        printf("child done on cpu %d\n", cpu_id);
    }
    return 0;
}

int main(int argc, char *argv[]) 
{
    int     number_of_loop;

    if (argc == 1)  number_of_loop = 1000;
    else            number_of_loop = atoi(argv[1]);
    if (measure_system_call(number_of_loop))    return 1;
    if (measure_context_swtich(number_of_loop)) return 1;
    return 0;
}
