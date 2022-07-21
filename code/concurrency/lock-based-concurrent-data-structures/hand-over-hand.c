# include <pthread.h>

typedef struct __node_t
{
    int                 key;
    struct  __node_t    *next;
    pthread_mutex_t lock;
} node_t;

typedef struct __list_t
{
    node_t          *head;
    pthread_mutex_t lock;
} list_t;

void    list_init(list_t *l)
{
    l->head = NULL;
}

void     list_insert(list_t *l, int key)
{
    node_t  *new;

    new = malloc(sizeof(node_t));
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

int     list_lookup(list_t *l, int key)
{
    int     rv;
    node_t  *cur;
    node_t  *prev;

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
