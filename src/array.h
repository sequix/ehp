#pragma once

#include <stdlib.h>

#define ARRAY_INIT_CAPACITY 8

typedef struct {
    size_t size;
    size_t cap;
    void **array;
} array_t;

array_t *array_new(void);

void array_free(array_t *arr);

#define array_size(arr) ((arr)->size)

#define array_get(arr, p) ((arr)->array[p])

#define array_set(arr, p, v) ((arr)->array[p] = (v))

#define array_front(arr) ((arr)->arr[0])

#define array_back(arr) ((arr)->arr[array_size(arr)-1])

int array_push_back(array_t *arr, void *elem);
