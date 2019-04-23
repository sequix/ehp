#pragma once

#include "str.h"
#include "types.h"

typedef unsigned map_hash_t;

typedef struct map_node_t {
    map_hash_t         hash;
    str_t             *key;
    str_t             *value;
    struct map_node_t *next;
} map_node_t;

typedef struct {
    len_t       nbuckets;
    map_node_t **buckets;
} map_t ;

map_t *map_new(len_t nbuckets);

// return 0 on success, negative on failure
int map_set(map_t *map, const str_t *key, const str_t *value);

// return pointer to the value, NULL if no such key
str_t *map_get(const map_t *map, const str_t *key);

// syntax candy allowing use c-style string as key and value
// But, I will not provide function return c-style char*,
// you should honor str_t.
#define map_set_c(m,k,v) map_set((m), str_new(k), str_new(v))
#define map_get_c(m,k) map_get((m), str_new(k))
