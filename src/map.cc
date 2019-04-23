#include "map.h"
#include "mem.h"
#include "log.h"

map_t *map_new(len_t nbuckets)
{
    map_t *map = talloc(map_t);
    if (map == NULL) {
        log_error("alloc map_t");
        return NULL;
    }
    map->buckets = stalloc(nbuckets, map_node_t*);
    if (map->buckets == NULL) {
        log_error("alloc %d map_t.buckets", nbuckets);
        return NULL;
    }
    map->nbuckets = nbuckets;
    return map;
}

static map_hash_t map_hash(const map_t *map, const str_t *str)
{
    map_hash_t hashval = 0;
    for (char *s = str->s; *s; ++s)
        hashval = *s + 31 * hashval;
    return hashval % map->nbuckets;
}

static map_node_t *map_new_node(const map_t *map,
        const str_t *key, const str_t *value)
{
    map_node_t *node = talloc(map_node_t);
    if (node == NULL) {
        log_error("alloc map_node_t for %s:%s", key, value);
        return NULL;
    }
    node->key = str_clone(key);
    node->value = str_clone(value);
    node->hash = map_hash(map, key);
    return node;
}

static map_node_t *map_getref(const map_t *map, const str_t *key)
{
    map_hash_t hash = map_hash(map, key);
    map_node_t *p = map->buckets[hash];

    while (p) {
        if (p->hash == hash && str_equal(p->key, key)) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

int map_set(map_t *map, const str_t *key, const str_t *value)
{
    if (key == NULL || value == NULL) {
        log_warn("map_set: null pointer");
        return -1;
    }
    map_node_t *p = map_getref(map, key);
    if (p != NULL) {
        p->value = str_clone(value);
        if (p->value == NULL) {
            return -2;
        }
        return 0;
    }
    p = map_new_node(map, key, value);
    if (p == NULL) {
        return -3;
    }
    p->next = map->buckets[p->hash];
    map->buckets[p->hash] = p;
    return 0;
}

str_t *map_get(const map_t *map, const str_t *key)
{
    map_node_t *p = map_getref(map, key);
    if (p != NULL) {
        return p->value;
    }
    return NULL;
}
