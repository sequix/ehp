#pragma once

#include "types.h"
#include "util.h"

typedef struct list_node_st {
    node_t value;
    list_node_st *prev;
    list_node_st *next;
} list_node_st;

typedef list_node_st* list_node_t;

typedef struct {
    len_t len;
    list_node_t head;
    list_node_t tail;
} list_st;

typedef list_st* list_t;

list_t list_new(void);

int list_push_front(list_t lst, node_t value);

node_t list_pop_front(list_t lst);

int list_push_back(list_t lst, node_t value);

node_t list_pop_back(list_t lst);
