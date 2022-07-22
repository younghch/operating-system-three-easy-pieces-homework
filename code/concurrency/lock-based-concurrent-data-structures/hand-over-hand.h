# include <pthread.h>

typedef struct __hand_over_hand_node_t
{
    int                 key;
    struct  __hand_over_hand_node_t    *next;
    pthread_mutex_t lock;
} hand_over_hand_node_t;

typedef struct __hand_over_hand_list_t
{
    hand_over_hand_node_t          *head;
    pthread_mutex_t lock;
} hand_over_hand_list_t;

void    hand_over_hand_list_init(hand_over_hand_list_t *l);

void    hand_over_hand_list_insert(hand_over_hand_list_t *l, int key);

int     hand_over_hand_list_lookup(hand_over_hand_list_t *l, int key);
