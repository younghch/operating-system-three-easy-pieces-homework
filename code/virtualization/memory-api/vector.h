#include <stdio.h>
#include <stdlib.h>

typedef struct vector
{
    int *array;
    int allocated_length;
    int length;
} vector; 

void vector_push(vector *v, int num) {
    if (v->array == NULL)
    {
        v->allocated_length = 5;
        v->array = (int*) malloc(sizeof(int) * (v->allocated_length));
        v->length = 1;
    }
    else if (v->allocated_length <= v->length)
    {
        v->allocated_length *= 2;
        int *doubled_array = (int*)realloc(v->array, sizeof(int) * v->allocated_length);
        v->array = doubled_array;
        v->length += 1;
    }
    else
        v->length += 1;
    v->array[v->length-1] = num;
}

int vector_pop(vector *v) {
    if (v->length == 0)
        return 0;
    v->length -= 1;
    return v->array[v->length];
}

void vector_print(vector *v)
{
    printf("print elements in vector:\n");
    for (int i=0; i < v->length; i++)
        printf("%d ", v->array[i]);
    printf("\n");
}

void vector_free(vector *v) {
    free(v->array);
    free(v);
}