#include "array.h"
#include <stdio.h>

array_t *array_new(void)
{
    array_t *arr = (array_t*)malloc(sizeof(array_t));
    if (arr == NULL) {
        return NULL;
    }
    arr->array = (void**)calloc(ARRAY_INIT_CAPACITY, sizeof(void*));
    if (arr->array == NULL) {
        return NULL;
    }
    arr->size = 0;
    arr->cap = ARRAY_INIT_CAPACITY;
    return arr;
}

void array_free(array_t *arr)
{
    free(arr->array);
    free(arr);
}

static int array_realloc(array_t *arr)
{
    int new_capacity = arr->cap << 1;
    arr->array = (void**)realloc(arr->array, new_capacity);
    if (arr->array == NULL) {
        return -1;
    }
    arr->cap = new_capacity;
    return 0;
}

int array_push_back(array_t *arr, void *elem)
{
    if (arr == NULL) {
        return -1;
    }
    if (arr->size >= arr->cap) {
        array_realloc(arr);
    }
    arr->array[arr->size++] = elem;
    return 0;
}
