#include "mem.h"
#include "log.h"
#include "array.h"

array_t *array_new(void)
{
    array_t *arr = talloc(array_t);
    if (arr == NULL) {
        log_error("alloc array_t");
        return NULL;
    }
    arr->array = stalloc(ARRAY_INIT_CAPACITY, void*);
    if (arr->array == NULL) {
        log_error("alloc array_t.array");
        return NULL;
    }
    arr->len = 0;
    arr->cap = ARRAY_INIT_CAPACITY;
    return arr;
}

static int array_realloc(array_t *arr)
{
    int new_cap = arr->cap << 1;
    arr->array = stralloc(arr->array, new_cap, void*);
    if (arr->array == NULL) {
        log_error("realloc array_t.array");
        return -1;
    }
    arr->cap = new_cap;
    return 0;
}

int array_push_back(array_t *arr, void *elem)
{
    if (arr == NULL) {
        log_warn("array_push_back: null pointer");
        return -1;
    }
    if (arr->len >= arr->cap && array_realloc(arr) < 0) {
        return -2;
    }
    arr->array[arr->len++] = elem;
    return 0;
}
