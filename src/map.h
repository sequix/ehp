#pragma once

#include <stdlib.h>

typedef unsigned ehp_map_hash_t;

struct ehp_map_node_t {
    ehp_map_hash_t hash;
    char *key;
    char *value;
    struct ehp_map_node_t *next;
};

typedef struct ehp_map_node_t ehp_map_node_t;

struct ehp_map_t {
    size_t nbuckets;
    ehp_map_node_t **buckets;
};

typedef struct ehp_map_t ehp_map_t;

ehp_map_t *ehp_map_new(size_t nbuckets);
void ehp_map_free(ehp_map_t *map);
int ehp_map_set(ehp_map_t *map, const char *key, const char *value);
char *ehp_map_get(const ehp_map_t *map, const char *key);
