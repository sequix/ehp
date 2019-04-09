#include "array.h"
#include <stdio.h>

ehp_array_t *ehp_array_new(void)
{
    ehp_array_t *arr = (ehp_array_t*)malloc(sizeof(ehp_array_t));
    if (arr == NULL) {
        return NULL;
    }
    arr->array = (void**)calloc(ARRAY_INIT_CAPACITY, sizeof(void*));
    if (arr->array == NULL) {
        return NULL;
    }
    arr->size = 0;
    arr->capacity = ARRAY_INIT_CAPACITY;
    return arr;
}

static int ehp_array_realloc(ehp_array_t *arr)
{
    int new_capacity = arr->capacity << 1;
    arr->array = (void**)realloc(arr->array, new_capacity);
    if (arr->array == NULL) {
        return -1;
    }
    arr->capacity = new_capacity;
    return 0;
}

int ehp_array_append(ehp_array_t *arr, void *elem)
{
    if (arr == NULL) {
        return -1;
    }
    if (arr->size >= arr->capacity) {
        ehp_array_realloc(arr);
    }
    arr->array[arr->size++] = elem;
    return 0;
}
