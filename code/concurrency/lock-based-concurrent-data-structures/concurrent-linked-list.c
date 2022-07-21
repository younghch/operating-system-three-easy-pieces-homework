# include <pthread.h>
# include "conccurrent-linked-list.h"

void    list_init(concurrent_list_t *l)
{
    l->head = NULL;
    pthread_mutex_init(&l->lock, NULL);
}

void     list_insert(concurrent_list_t *l, int key)
{
    concurent_node_t  *new;

    new = malloc(sizeof(concurent_node_t));
    if (new == NULL) {
        perror("malloc");
        return;
    }

    new->key = key;
    pthread_mutex_lock(&l->lock);
    new->next = l->head;
    l->head = new;
    pthread_mutex_unlock(&l->lock);
}

int     list_lookup(concurrent_list_t *l, int key)
{
    int     rv;
    concurent_node_t  *cur;

    rv = -1;
    pthread_mutex_lock(&l->lock);
    cur = l->head;
    while (cur)
    {
        if (cur->key == key)
        {
            rv = 0;
            break;
        }
        cur = cur->next;
    }
    pthread_mutex_unlock(&l->lock);
    return rv;
}
