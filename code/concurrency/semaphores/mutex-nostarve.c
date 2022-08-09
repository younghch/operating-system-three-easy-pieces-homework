#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "common_threads.h"

//
// Here, you have to write (almost) ALL the code. Oh no!
// How can you show that a thread does not starve
// when attempting to acquire this mutex you build?
//

typedef struct __ns_mutex_t {
    int     room1;
    int     room2;
    sem_t   mutex;
    sem_t   t1;
    sem_t   t2;
} ns_mutex_t;

ns_mutex_t m;

void ns_mutex_init(ns_mutex_t *m) {
    m->room1 = 0;
    m->room2 = 0;
    Sem_init(&m->mutex, 1);
    Sem_init(&m->t1, 1);
    Sem_init(&m->t2, 0)
}

void ns_mutex_acquire(ns_mutex_t *m) {
    Sem_wait(&m->mutex);
    m->room1 += 1;
    Sem_post(&m->mutex);
    
    sleep(2); // give time for all threads to enter the room1
    Sem_wait(&m->t1);
    m->room2 += 1;
    Sem_wait(&m->mutex);
    m->room1 -= 1;
    if (m->room1 == 0)
    {
        Sem_post(&m->mutex);
        Sem_post(&m->t2);
    }
    else
    {
        Sem_post(&m->mutex);
        Sem_post(&m->t1);
    }
    Sem_wait(&m->t2);
}

void ns_mutex_release(ns_mutex_t *m) {
    m->room2 -= 1;
    if (m->room2 == 0)
    {
        Sem_post(&m->t1);
    }
    else
        Sem_post(&m->t2);
}

typedef struct __worker_params_t {
    int     id;
    char    is_first;
} worker_params_t;

void *worker(void *arg) {
    worker_params_t *w_arg = (worker_params_t *) arg;
    char            *type = w_arg->is_first == 0 ? "first" : "later";

    printf("%s thread %d: created\n", type, w_arg->id);
    ns_mutex_acquire(&m);
    printf("%s thread %d: entered\n", type, w_arg->id);
    ns_mutex_release(&m);
    return NULL;
}

int main(int argc, char *argv[]) {
    int             i;
    int             num_firsts;
    int             num_lates;
    long long int   id;
    pthread_t       *firsts;
    pthread_t       *lates;
    worker_params_t *w_args;

    assert(argc == 3);
    num_firsts = atoi(argv[1]);
    num_lates = atoi(argv[2]);
    firsts = malloc(sizeof(pthread_t)*num_firsts);
    lates = malloc(sizeof(pthread_t)*num_lates);
    w_args = malloc(sizeof(worker_params_t)*(num_firsts+num_lates));

    ns_mutex_init(&m);

    printf("parent: begin\n");
    for (i = 0; i < num_firsts; i++)
    {
        w_args[i].id = i;
        w_args[i].is_first = 0;
	    Pthread_create(&firsts[i], NULL, worker, (void *)&w_args[i]);
    }
    sleep(2);
    for (i = 0; i < num_lates; i++)
    {
        w_args[num_firsts+i].id = i;
        w_args[num_firsts+i].is_first = 1;
	    Pthread_create(&lates[i], NULL, worker, (void *)&w_args[num_firsts+i]);
    }
    for (i = 0; i < num_firsts; i++)
	    Pthread_join(firsts[i], NULL);
    for (i = 0; i < num_lates; i++)
	    Pthread_join(lates[i], NULL);
    printf("parent: end\n");
    free(firsts);
    free(lates);
    free(w_args);
    return 0;
}

