#pragma once

#include "types.h"
#include "str.h"

typedef struct lru_node_st {
    len_t len;
    str_t key;
    str_t value;
    struct lru_node_st *next;
    struct lru_node_st *prev;
} lru_node_st;

typedef lru_node_st* lru_node_t;

// NOT thread-safe
typedef struct {
    len_t cap;
    len_t len;

    len_t hash_cap;
    len_t hash_capm1;
    len_t hash_factor;
    lru_node_t *hash_bks;

    lru_node_t list_head;
    lru_node_t list_tail;
} lru_st;

typedef lru_st* lru_t;

lru_t lru_new(len_t cap);

int lru_set(lru_t lru, str_t key, str_t value);

str_t lru_get(lru_t lru, str_t key);

int lru_set_from_file(lru_t lru, str_t key, str_t filename);
