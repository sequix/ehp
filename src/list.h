#pragma once

#include <stdlib.h>
#include <stdbool.h>

#define LIST_INIT_CAPACITY 16

typedef struct {
    // how many elements we have now
    size_t size;

    // how many elements we can hold now
    size_t capacity;

    // index to the first element
    int front_index;

    // index to the last element
    int back_index;

    // the next element of elems[i] is elems[next[i]]
    int *next;

    // the elements, and the index we use inside starts from 1
    void  **elems;
} list_t;

// index starts from 0
// return true if you do not iterate more
typedef bool (*list_iter_func_t)(int index, void *elem);

list_t *list_new(void);

void list_free(list_t *list);

void *list_front(const list_t *list);

void *list_back(const list_t *list);

int list_push_front(list_t *list, void *elem);

int list_push_back(list_t *list, void *elem);

int list_iter(const list_t *list, list_iter_func_t func);
