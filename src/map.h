#pragma once

#include <stdlib.h>

typedef unsigned map_hash_t;

typedef struct map_node_t {
    map_hash_t         hash;
    char              *key;
    char              *value;
    struct map_node_t *next;
} map_node_t;

typedef struct {
    size_t       nbuckets;
    map_node_t **buckets;
} map_t ;

map_t *map_new(size_t nbuckets);

void map_free(map_t *map);

int map_set(map_t *map, const char *key, const char *value);

// given "key: value"
int map_set_from_line(map_t *map, char *s);

char *map_get(const map_t *map, const char *key);
