#pragma once

#include "str.h"
#include "types.h"

typedef unsigned map_hash_t;

typedef struct map_node_st {
    str_t              key;
    str_t              value;
    struct map_node_st *next;
} map_node_st;

typedef map_node_st* map_node_t;

typedef struct {
    len_t       nbuckets;
    map_node_t *buckets;
} map_st;

typedef map_st* map_t;

map_t map_new(len_t nbuckets);

// return 0 on success, negative on failure
int map_set(map_t map, str_t key, str_t value);

// return pointer to the value, NULL if no such key
str_t map_get(map_t map, str_t key);
