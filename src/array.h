#pragma once

#include <stdlib.h>

#define ARRAY_INIT_CAPACITY 16

typedef struct {
    size_t size;
    size_t capacity;
    void **array;
} array_t;

#define array_size(x) ((x)->size)
#define array_get(x, p) ((x)->array[p])

array_t *array_new(void);

int array_push_back(array_t *arr, void *elem);
