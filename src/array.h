#pragma once

#include "types.h"

typedef struct {
    len_t len;
    len_t cap;
    void **array;
} array_st;

typedef array_st* array_t;

array_t array_new(len_t cap);

#define array_len(arr) ((arr)->len)

#define array_get(arr,p)        ((arr)->array[p])
#define array_get_(T,arr,p)     ((T)((arr)->array[p]))
#define array_get_str(arr,p)    (array_get_(str_t,arr,p))
#define array_get_int(arr,p)    (*array_get_(int*,arr,p))
#define array_get_short(arr,p)  (*array_get_(short*,arr,p))
#define array_get_long(arr,p)   (*array_get_(long*,arr,p))
#define array_get_char(arr,p)   (*array_get_(char*,arr,p))
#define array_get_float(arr,p)  (*array_get_(float*,arr,p))
#define array_get_double(arr,p) (*array_get_(double*,arr,p))

#define array_set(arr,p,v) ((arr)->array[p] = (void*(v)))

#define array_push_back(arr,v) array_push_back_actual((arr),((void*)(v)))

int array_push_back_actual(array_t arr, void *v);
