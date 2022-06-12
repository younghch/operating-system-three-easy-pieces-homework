#include <stdlib.h>
#include <stdio.h>
#include "vector.h"

int main(int argc, char *argv[]) 
{
    vector *v;
    
    v = malloc(sizeof(vector));
    
    v->array = NULL;
    v->allocated_length = 0;
    v->length = 0;

    vector_push(v, 1);
    vector_push(v, 2);
    vector_push(v, 3);
    vector_push(v, 4);
    
    vector_print(v);

    vector_push(v, 5);
    vector_push(v, 6);
    vector_push(v, 7);
    
    vector_print(v);
    
    vector_pop(v);
    vector_pop(v);

    vector_print(v);

    vector_free(v);
}
