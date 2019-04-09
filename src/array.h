#pragma once

#include <stdlib.h>

#define ARRAY_INIT_CAPACITY 16

typedef struct {
    size_t size;
    size_t capacity;
    void **array;
} ehp_array_t;

#define ehp_array_size(x) ((x)->size)
#define ehp_array_get(x, p) ((x)->array[p])
#define ehp_array_last(x) ((x)->array[(x)->size-1])

ehp_array_t *ehp_array_new(void);
int ehp_array_append(ehp_array_t *arr, void *elem);
