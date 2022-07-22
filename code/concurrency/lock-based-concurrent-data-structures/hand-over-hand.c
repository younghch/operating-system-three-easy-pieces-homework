#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "hand-over-hand.h"

void    hand_over_hand_list_init(hand_over_hand_list_t *l)
{
    l->head = NULL;
}

void     hand_over_hand_list_insert(hand_over_hand_list_t *l, int key)
{
    hand_over_hand_node_t   *new;

    new = malloc(sizeof(hand_over_hand_node_t));
    if (new == NULL) {
        perror("malloc");
        return;
    }
    pthread_mutex_init(&new->lock, NULL);

    new->key = key;
    pthread_mutex_lock(&l->lock);
    new->next = l->head;
    l->head = new;
    pthread_mutex_unlock(&l->lock);
}

int     hand_over_hand_list_lookup(hand_over_hand_list_t *l, int key)
{
    int                     rv;
    hand_over_hand_node_t   *cur;
    hand_over_hand_node_t   *prev;

    rv = -1;
    cur = l->head;
    prev = NULL;
    while (cur)
    {
        pthread_mutex_lock(&cur->lock);
        if (prev) pthread_mutex_unlock(&prev->lock);
        prev = cur;
        cur = cur->next;
        if (prev->key == key)
        {
            rv = 0;
            break;
        }
    }
    pthread_mutex_unlock(&prev->lock);
    return rv;
}
