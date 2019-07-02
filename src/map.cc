#include "map.h"
#include "mem.h"
#include "log.h"
#include <stdlib.h>

#define MAP_INIT_CAP 16

map_t map_new(void)
{
    map_t map = talloc(map_st);
    if (!map) {
        log_error("map_new(): talloc");
        exit(1);
    }
    map->len = 0;
    map->cap = MAP_INIT_CAP;
    map->capm1 = MAP_INIT_CAP - 1;
    map->factor = (MAP_INIT_CAP>>1) + (MAP_INIT_CAP>>2);

    map->bks = stalloc(MAP_INIT_CAP, map_node_t);
    if (!map->bks) {
        log_error("map_new(): stalloc");
        exit(1);
    }
    return map;
}

static int map_set_actual(map_t map, map_node_t node)
{
    map_node_t *bks = map->bks;
    map_hash_t hash = str_hash(node->key) & map->capm1;
    map_hash_t i = hash;

    do {
        if (!bks[i] || str_eq(node->key, bks[i]->key)) {
            bks[i] = node;
            return 0;
        }
        i = (i + 1) & map->capm1;
    } while (i != hash);
    return -1;
}

// TODO: mimic redis, realloc on the fly
static int map_realloc(map_t map)
{
    len_t old_cap = map->cap;
    map_node_t *old_bks = map->bks;

    len_t new_cap = old_cap << 1;
    map_node_t *new_bks = stalloc(new_cap, map_node_t);
    if (!new_bks) {
        log_error("map_realloc: stalloc");
        exit(2);
    }
    map->bks = new_bks;
    map->cap = new_cap;
    map->capm1 = new_cap - 1;
    map->factor = (new_cap>>1) + (new_cap>>2);

    len_t cnt = 0;
    for (len_t i = 0; i < old_cap; ++i) {
        if (!map->bks[i]) continue;
        if (map_set_actual(map, old_bks[i]) < 0) {
            return -1;
        }
        if (++cnt >= map->len) break;
    }
    return 0;
}

static map_node_t map_node_new(str_t key, str_t value)
{
    map_node_t node = talloc(map_node_st);
    if (!node) {
        log_error("map_node_new: talloc");
        exit(1);
    }
    node->key = key;
    node->value = value;
    return node;
}

int map_set(map_t map, str_t key, str_t value)
{
    if (map->len >= map->factor) {
        if (map_realloc(map) < 0) {
            return -1;
        }
    }

    map_node_t node = map_node_new(key, value);
    if (!node) {
        return -1;
    }

    if (map_set_actual(map, node) < 0) {
        return -1;
    }
    ++map->len;
    return 0;
}

static map_node_t map_getref(map_t map, str_t key)
{
    map_hash_t hash = str_hash(key) & map->capm1;
    map_hash_t i = hash;

    do {
        if (map->bks[i] && str_eq(map->bks[i]->key, key)) {
            return map->bks[i];
        }
        i = (i + 1) & map->capm1;
    } while (i != hash);
    return NULL;
}

str_t map_get(map_t map, str_t key)
{
    map_node_t p = map_getref(map, key);
    return (p) ? p->value : NULL;
}

void map_iter(map_t map, map_iter_func_t func)
{
    len_t cnt = 0;
    for (len_t i = 0; i < map->cap; ++i) {
        if (!map->bks[i]) continue;
        func(map->bks[i]->key, map->bks[i]->value);
        if (++cnt >= map->len) break;
    }
}

uint64_t map_get_uint64(map_t map, str_t key)
{
    str_t vs = map_get(map, key);
    return atoll(str_tocc(vs));
}

static int map_print_helper(str_t key, str_t value)
{
    str_printf("%r: %r\n", key, value);
    return 0;
}

void map_print(map_t map)
{
    map_iter(map, map_print_helper);
}
