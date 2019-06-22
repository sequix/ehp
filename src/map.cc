#include "map.h"
#include "mem.h"
#include "log.h"
#include <stdlib.h>

map_t map_new(len_t nbuckets)
{
    map_t map = talloc(map_st);
    if (map == NULL) {
        return NULL;
    }
    map->buckets = stalloc(nbuckets, map_node_t);
    if (map->buckets == NULL) {
        return NULL;
    }
    map->nbuckets = nbuckets;
    return map;
}

static map_node_t map_new_node(str_t key, str_t value)
{
    map_node_t node = talloc(map_node_st);
    if (node == NULL) {
        return NULL;
    }
    node->key = key;
    node->value = value;
    return node;
}

static map_node_t map_getref(map_t map, str_t key)
{
    map_hash_t hash = str_hash(key) % map->nbuckets;
    map_node_t p = map->buckets[hash];

    while (p) {
        if (str_eq(p->key, key)) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

int map_set(map_t map, str_t key, str_t value)
{
    if (str_isempty(key) || str_isempty(value)) {
        return -1;
    }
    map_node_t p = map_getref(map, key);
    if (p != NULL) {
        p->value = value;
        return 0;
    }
    p = map_new_node(key, value);
    if (p == NULL) {
        return -2;
    }
    hash_t hash = str_hash(key) % map->nbuckets;
    p->next = map->buckets[hash];
    map->buckets[hash] = p;
    return 0;
}

str_t map_get(map_t map, str_t key)
{
    map_node_t p = map_getref(map, key);
    if (p != NULL) {
        return p->value;
    }
    return NULL;
}

void map_iter(map_t map, map_iter_func_t func)
{
    for (len_t i = 0; i < map->nbuckets; ++i) {
        if (map->buckets[i] == NULL) continue;
        for(map_node_t p = map->buckets[i]; p; p = p->next) {
            if (func(p->key, p->value)) {
                return;
            }
        }
    }
}

uint64_t map_get_uint64(map_t map, str_t key)
{
    str_t vs = map_get(map, key);
    return atoll(str_tocc(vs));
}
