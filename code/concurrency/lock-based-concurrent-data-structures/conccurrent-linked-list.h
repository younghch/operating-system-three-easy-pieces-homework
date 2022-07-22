# include <pthread.h>

typedef struct __concurrent_node_t
{
    int                 key;
    struct  __concurrent_node_t    *next;     
} concurent_node_t;

typedef struct __concurrent_list_t
{
    concurent_node_t          *head;
    pthread_mutex_t lock;
} concurrent_list_t;

void    concurrent_list_init(concurrent_list_t *l);
void    concurrent_list_insert(concurrent_list_t *l, int key);
int     concurrent_list_lookup(concurrent_list_t *l, int key);