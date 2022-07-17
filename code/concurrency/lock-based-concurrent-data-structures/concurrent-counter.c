#define _GNU_SOURCE 
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <unistd.h>
#include "measure-time.h"

typedef struct __counter_t {
    int             value;
    pthread_mutex_t lock;
} counter_t;

typedef struct __worker_params {
    counter_t   *counter;
    int         count;
} worker_params;

void init(counter_t *counter) {
    counter->value = 0;
    assert(pthread_mutex_init(&counter->lock, NULL) == 0);
}

void increment(counter_t *counter, int loop){
	for (int i = 0; i < loop; i++) {
        assert(pthread_mutex_lock(&counter->lock) == 0);
    	counter->value++;
        assert(pthread_mutex_unlock(&counter->lock) == 0);
    }
}

void *worker(void *args) {
	worker_params   *w_args = (worker_params *) args;
    increment(w_args->counter, w_args->count);
    return NULL;
}

int		main(int argc, char *argv[]) 
{
    int             max_num_of_cpu;
    int			    num_of_threads;
    int			    count;
    char            one_thread_per_cpu;
	pthread_t	    *threads;
    pthread_attr_t  *thread_attrs;
    cpu_set_t       *cpu_sets;
    counter_t	    counter;
    worker_params   w_args;

	if (argc != 4) {
        printf ("please enter three arguments : number of threads, increase count, one_thread_per_cpu\n");
        return -1;
    }

    num_of_threads= atoi(argv[1]);
    count = atoi(argv[2]);
    one_thread_per_cpu = strcmp(argv[3], "true") == 0 ? 1 : 0;
    max_num_of_cpu = sysconf(_SC_NPROCESSORS_CONF);
    if (one_thread_per_cpu == 1) assert( num_of_threads <= max_num_of_cpu);
    threads = malloc(sizeof(pthread_t)*num_of_threads);
    thread_attrs = malloc(sizeof(pthread_attr_t)*num_of_threads);
    cpu_sets = malloc(sizeof(cpu_set_t)*max_num_of_cpu);
    assert(threads != NULL && thread_attrs != NULL && cpu_sets != NULL);

	init(&counter);
    w_args.counter = &counter;
    w_args.count = count;

    start_timer();
	for (int i = 0; i < num_of_threads; i++)
    {
        CPU_ZERO(cpu_sets+i);
        CPU_SET(i, cpu_sets+i);
        pthread_attr_init(thread_attrs+i);
        if (one_thread_per_cpu == 1) 
            pthread_attr_setaffinity_np(thread_attrs+i, sizeof(cpu_set_t), cpu_sets+i);
        else
            // bind thread to first cpu
            pthread_attr_setaffinity_np(thread_attrs+i, sizeof(cpu_set_t), cpu_sets);
        pthread_create(threads+i, thread_attrs+i, worker, &w_args);
    }
    for (int i = 0; i < num_of_threads; i++)
		pthread_join(threads[i], NULL);
    end_timer();
    printf(one_thread_per_cpu == 1 ? "one thread per cpu\n" : "running on only one cpu\n");
    printf("number of threads: %d increase count each: %d total time %fs\n", num_of_threads, count, get_elapsed_seconds());
}
