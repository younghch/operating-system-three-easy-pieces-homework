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
    int             threshold;
    int             num_of_cpu;
    int             global_value;
    pthread_mutex_t global_lock;
    int             *local_values;
    pthread_mutex_t *local_locks;

    
} counter_t;

typedef struct __worker_params {
    counter_t   *counter;
    int         count;
    int         cpu_id;
} worker_params;

void init(counter_t *counter, int threshold) {
    counter->threshold = threshold;
    counter->num_of_cpu = sysconf(_SC_NPROCESSORS_CONF);
    counter->global_value = 0;
    counter->local_values = malloc(sizeof(int)*counter->num_of_cpu);
    counter->local_locks = malloc(sizeof(pthread_mutex_t)*counter->num_of_cpu);
    assert(counter->local_values != NULL && counter->local_locks != NULL);
    assert(pthread_mutex_init(&counter->global_lock, NULL) == 0);
    for (int i=0; i < counter->num_of_cpu; i++){
        assert(pthread_mutex_init(counter->local_locks+i, NULL) == 0);
        counter->local_values[i] = 0;
    }
}

void increment(counter_t *counter, int cpu_id) {
    assert(pthread_mutex_lock(counter->local_locks+cpu_id) == 0);
	counter->local_values[cpu_id]++;
    if (counter->local_values[cpu_id] >= counter->threshold){
        assert(pthread_mutex_lock(&counter->global_lock) == 0);
        counter->global_value += counter->local_values[cpu_id];
        assert(pthread_mutex_unlock(&counter->global_lock) == 0);
        counter->local_values[cpu_id] = 0;
    }
    assert(pthread_mutex_unlock(counter->local_locks+cpu_id) == 0);
}

int get_value(counter_t *counter) {
    int global_value;
    assert(pthread_mutex_lock(&counter->global_lock) == 0);
    global_value = counter->global_value;
    assert(pthread_mutex_unlock(&counter->global_lock) == 0);
    return global_value;
}

void *worker(void *args) {
	worker_params   *w_args = (worker_params *) args;
    for (int i = 0; i < w_args->count; i++) increment(w_args->counter, w_args->cpu_id);
    return NULL;
}

void worker_params_init(worker_params *w_args, counter_t *counter, int count, int cpu_id){
    w_args->counter = counter;
    w_args->count = count;
    w_args->cpu_id = cpu_id;
}

int		main(int argc, char *argv[]) 
{
    int             num_of_cpus;
    int			    num_of_threads;
    int			    count;
    int             threshold;
	pthread_t	    *threads;
    pthread_attr_t  *thread_attrs;
    cpu_set_t       *cpu_sets;
    counter_t	    counter;
    worker_params   *w_args;

	if (argc != 4) {
        printf ("please enter three arguments : number of threads, increase count, threshold value\n");
        return -1;
    }

    num_of_cpus = sysconf(_SC_NPROCESSORS_CONF);
    num_of_threads= atoi(argv[1]);
    count = atoi(argv[2]);
    threshold = atoi(argv[3]);
    threads = malloc(sizeof(pthread_t)*num_of_threads);
    thread_attrs = malloc(sizeof(pthread_attr_t)*num_of_cpus);
    cpu_sets = malloc(sizeof(cpu_set_t)*num_of_cpus);
    w_args = malloc(sizeof(worker_params)*num_of_cpus);
    assert(threads != NULL && thread_attrs != NULL && cpu_sets != NULL);

	init(&counter, threshold);
    for (int i = 0; i < num_of_cpus; i++){
        CPU_ZERO(cpu_sets+i);
        CPU_SET(i, cpu_sets+i);
        worker_params_init(w_args+i, &counter, count/num_of_threads, i);
        pthread_attr_init(thread_attrs+i);
    }
    start_timer();
	for (int i = 0; i < num_of_threads; i++)
    {
        int cpu_id = i%num_of_cpus;
        pthread_attr_setaffinity_np(thread_attrs+cpu_id, sizeof(cpu_set_t), cpu_sets+i);
        pthread_create(threads+i, thread_attrs+cpu_id, worker, w_args+cpu_id);
        pthread_create(threads+i, NULL, worker, &w_args[cpu_id]);
    }
    for (int i = 0; i < num_of_threads; i++)
		pthread_join(threads[i], NULL);
    end_timer();
    printf("number of threads: %d, total increase count: %d, total time %fs, global value : %d\n", num_of_threads, count, get_elapsed_seconds(), get_value(&counter));

    for (int i=0; i < num_of_cpus; i++)
        pthread_attr_destroy(thread_attrs+i);
    free(threads);
    free(thread_attrs);
    free(counter.local_values);
    free(counter.local_locks);
    free(cpu_sets);
    free(w_args);
}
