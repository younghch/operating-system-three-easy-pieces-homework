# include <pthread.h>
# include "hand-over-hand.h"

void    list_init(hand_over_hand_list_t *l)
{
    l->head = NULL;
}

void     list_insert(hand_over_hand_list_t *l, int key)
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

int     list_lookup(hand_over_hand_list_t *l, int key)
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
        if (cur->key == key)
        {
            rv = 0;
            break;
        }
        prev = cur;
        cur = cur->next;
    }
    pthread_mutex_unlock(&prev->lock);
    return rv;
}
