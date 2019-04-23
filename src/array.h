#pragma once

#include "types.h"

#define ARRAY_INIT_CAPACITY 8

typedef struct {
    len_t len;
    len_t cap;
    void **array;
} array_t;

array_t *array_new(void);

#define array_get(arr,p) ((arr)->array[p])

#define array_set(arr,p,v) ((arr)->array[p] = (v))

int array_push_back(array_t *arr, void *elem);
