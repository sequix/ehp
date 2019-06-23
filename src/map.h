#pragma once

#include "str.h"
#include "types.h"

typedef unsigned map_hash_t;

typedef struct map_node_st {
    str_t key;
    str_t value;
} map_node_st;

typedef map_node_st* map_node_t;

typedef struct {
    // how many elements in the map
    len_t       len;

    // capacity is always the exponentials of 2
    len_t       cap;

    // capacity minus 1, for hash calculation
    len_t       capm1;

    // factor = cap * 3/4 = (cap>>2) + (cap>>1)
    len_t       factor;

    // buckets
    map_node_t* bks;
} map_st;

typedef map_st* map_t;

map_t map_new(void);

// return 0 on success, negative on failure
int map_set(map_t map, str_t key, str_t value);

// return pointer to the value, NULL if no such key
str_t map_get(map_t map, str_t key);

// how many elements the map x holds
#define map_len(x) ((x)->len)

// function called upon every element
typedef int (*map_iter_func_t)(str_t key, str_t value);

// iterate all the element in map
void map_iter(map_t map, map_iter_func_t func);

// syntax candy allowing use c-style string as key and value
// But, I will not provide function return c-style char*,
// you should honor str_t.
#define map_set_c(m,k,v) map_set((m), str_from(k), str_from(v))
#define map_get_c(m,k) map_get((m), str_from(k))

uint64_t map_get_uint64(map_t map, str_t key);

void map_print(map_t map);
