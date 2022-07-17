#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
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
    assert(pthread_mutex_lock(&counter->lock) == 0);
	for (int i = 0; i < loop; i++)
		counter->value++;
    assert(pthread_mutex_unlock(&counter->lock) == 0);
}

void *worker(void *args) {
	worker_params   *w_args = (worker_params *) args;
    increment(w_args->counter, w_args->count);
    return NULL;
}

int		main(int argc, char *argv[]) 
{
    int			    num_of_threads;
    int			    count;
	pthread_t	    *threads;
    counter_t	    counter;
    worker_params   w_args;

	if (argc != 3) {
        printf ("please enter two arguments : number of threads, increase count");
        return -1;
    }

    num_of_threads= atoi(argv[1]);
    count = atoi(argv[2]);
    threads = malloc(sizeof(pthread_t)*num_of_threads);
    assert(threads != NULL);

	init(&counter);
    w_args.counter = &counter;
    w_args.count = count;

    start_timer();
	for (int i = 0; i < num_of_threads; i++)
		pthread_create(threads+i, NULL, worker, &w_args);
    for (int i = 0; i < num_of_threads; i++)
		pthread_join(threads[i], NULL);
    end_timer();
    printf("time took running %d threads increaising counter %d times each: %fs\n", num_of_threads, count, get_elapsed_seconds());
}
