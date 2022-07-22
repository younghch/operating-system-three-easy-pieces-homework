#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "measure-time.h"
#include "conccurrent-linked-list.h"
#include "hand-over-hand.h"

pthread_barrier_t	start_barrier;
int					size;
int					num_of_threads;
int					num_of_search;


typedef struct __worker_args {
    hand_over_hand_list_t	*h;
	concurrent_list_t		*c;
} worker_args;


void	init_lists(hand_over_hand_list_t *h, concurrent_list_t *c, int size)
{
	concurrent_list_init(c);
	hand_over_hand_list_init(h);
	
	for (int i = 0; i < size; i++)
	{
		concurrent_list_insert(c, i);
		hand_over_hand_list_insert(h, i);
	}
}

void 	*search_linearly(void *args) {
	worker_args   *w_args = (worker_args *) args;
    pthread_barrier_wait(&start_barrier);
	if (w_args->h != NULL)
		for (int i = 0; i < num_of_search; i++)
			hand_over_hand_list_lookup(w_args->h, i%size);
	else
		for (int i = 0; i < num_of_search; i++)
			concurrent_list_lookup(w_args->c, i%size);
    return NULL;
}

void 	*search_randomly(void *args) {
	worker_args   *w_args = (worker_args *) args;
    pthread_barrier_wait(&start_barrier);
	if (w_args->h != NULL)
		for (int i = 0; i < num_of_search; i++)
			hand_over_hand_list_lookup(w_args->h, random()%size);
	else
		for (int i = 0; i < num_of_search; i++)
			concurrent_list_lookup(w_args->c, random()%size);
    return NULL;
}

void	run_simulation(void *(search)(void *), hand_over_hand_list_t *h, concurrent_list_t *c, char *title)
{
	worker_args w_args;
	pthread_t	threads[num_of_threads];
	
	w_args.h = h;
    w_args.c = c;
    pthread_barrier_init(&start_barrier, NULL, num_of_threads);
	for (int i = 0; i < num_of_threads; i++)
        pthread_create(&threads[i], NULL, search, &w_args);
    start_timer();
    for (int i = 0; i < num_of_threads; i++)
		pthread_join(threads[i], NULL);
	end_timer();
    pthread_barrier_destroy(&start_barrier);
	printf("%s\ntime: %fs\n\n", title, get_elapsed_seconds());
}

int		main(int argc, char *argv[]) 
{
	hand_over_hand_list_t	h;
	concurrent_list_t		c;
	pthread_t	    		*threads;


	if (argc != 4) {
        printf ("please enter two argument : size of the list, number of threads, number_of_search\n");
        return -1;
    }

	size = atoi(argv[1]);
    num_of_threads = atoi(argv[2]);
	num_of_search = atoi(argv[3]) / num_of_threads;
	init_lists(&h, &c, size);

    printf("running test for \nlist size: %d\nnumber of threads: %d\nnum_of_search: %s\n\n", size, num_of_threads, argv[3]);
	run_simulation(search_linearly, &h, NULL, "linear search for hand-over-hand list");
	run_simulation(search_linearly, NULL, &c, "linear search for concurrent-linked list");
	run_simulation(search_randomly, &h, NULL, "random search for hand-over-hand list");
	run_simulation(search_randomly, NULL, &c, "random search for concurrent-linked list");
	
}