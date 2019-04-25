#include "mem.h"
#include "log.h"
#include "array.h"

array_t array_new(len_t cap)
{
    array_t arr = talloc(array_st);
    if (arr == NULL) {
        return NULL;
    }
    arr->array = stalloc(cap, void*);
    if (arr->array == NULL) {
        return NULL;
    }
    arr->len = 0;
    arr->cap = cap;
    return arr;
}

static int array_realloc(array_t arr)
{
    int new_cap = arr->cap << 1;
    arr->array = stralloc(arr->array, new_cap, void*);
    if (arr->array == NULL) {
        return -1;
    }
    arr->cap = new_cap;
    return 0;
}

int array_push_back_actual(array_t arr, void *v)
{
    if (arr == NULL) {
        return -1;
    }
    if (arr->len >= arr->cap && array_realloc(arr) < 0) {
        return -2;
    }
    arr->array[arr->len++] = v;
    return 0;
}
