#include "list.h"

list_t *list_new(void)
{
    list_t *list = (list_t*)malloc(sizeof(list_t));
    if (list == NULL) {
        return NULL;
    }
    
    list->next = (int*)calloc(LIST_INIT_CAPACITY, sizeof(int));
    if (list->next == NULL) {
        return NULL;
    }

    list->elems = (void**)calloc(LIST_INIT_CAPACITY, sizeof(void*));
    if (list->elems == NULL) {
        return NULL;
    }

    list->size = 0;
    list->front_index = 0;
    list->back_index = 0;
    list->capacity = LIST_INIT_CAPACITY;
    return list;
}

void list_free(list_t *list)
{
    if (list == NULL) {
        return;
    }
    free(list->next);
    free(list->elems);
    free(list);
}

void *list_front(const list_t *list)
{
    if (list == NULL || list->size <= 0) {
        return NULL;
    }
    return list->elems[list->front_index];
}

void *list_back(const list_t *list)
{
    if (list == NULL || list->size <= 0) {
        return NULL;
    }
    return list->elems[list->back_index];
}

static int list_realloc(list_t *list)
{
    int new_capacity = list->capacity << 1;
    list->next = (int*)realloc(list->next, new_capacity * sizeof(int));
    if (list->next == NULL) {
        return -1;
    }
    list->elems = (void**)realloc(list->elems, new_capacity * sizeof(void*));
    if (list->next == NULL) {
        return -1;
    }
    list->capacity = new_capacity;
    return 0;
}

int list_push_front(list_t *list, void *elem)
{
    if (list == NULL) {
        return 0;
    }
    if (list->size >= list->capacity) {
        if (list_realloc(list) < 0) {
            return -1;
        }
    }
    int ni = list->size + 1;
    list->elems[ni] = elem;
    list->next[ni] = list->front_index;
    list->front_index = ni;
    if (++list->size == 1) {
        list->back_index = ni;
    }
    return 0;
}

int list_push_back(list_t *list, void *elem)
{
    if (list == NULL) {
        return 0;
    }
    if (list->size >= list->capacity) {
        if (list_realloc(list) < 0) {
            return -1;
        }
    }
    int ni = list->size + 1;
    list->elems[ni] = elem;
    list->next[ni] = 0;
    list->next[list->back_index] = ni;
    list->back_index = ni;
    if (++list->size == 1) {
        list->front_index = ni;
    }
    return 0;
}

int list_iter(const list_t *list, list_iter_func_t func)
{
    if (list == NULL) {
        return -1;
    }
    int index = 0;
    int *next = list->next;
    void **elems = list->elems;
    for (int p = list->front_index; p; p = next[p]) {
        if (func(index++, elems[p])) {
            break;
        }
    }
    return 0;
}
