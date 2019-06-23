#include "map.h"
#include "mem.h"
#include "log.h"
#include <stdlib.h>

#define MAP_INIT_CAP 64

map_t map_new(void)
{
    map_t map = talloc(map_st);
    if (!map) {
        log_error("map_new(): talloc");
        exit(2);
    }
    map->len = 0;
    map->cap = MAP_INIT_CAP;
    map->capm1 = MAP_INIT_CAP - 1;
    map->factor = (MAP_INIT_CAP>>1) + (MAP_INIT_CAP>>2);

    map->bks = stalloc(MAP_INIT_CAP, map_node_t);
    if (!map->bks) {
        log_error("map_new(): stalloc");
        exit(2);
    }
    return map;
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

static void map_insert(map_node_t node, map_node_t *bks, len_t capm1)
{
    map_hash_t hash = str_hash(node->key) & capm1;
    map_hash_t i = hash;

    do {
        if (!bks[i]) {
            bks[i] = node;
            return;
        }
        i = (i + 1) & capm1;
    } while (i != hash);
}

static void map_insert_kv(str_t key, str_t value, map_node_t *bks, len_t capm1)
{
    map_node_t node = talloc(map_node_st);
    if (!node) {
        log_error("map_insert_kv: node alloc");
        exit(2);
    }
    node->key = key;
    node->value = value;
    map_insert(node, bks, capm1);
}

// TODO: mimic redis, realloc on the fly
static void map_realloc(map_t map)
{
    len_t new_cap = map->cap << 1;
    len_t new_capm1 = new_cap - 1;
    len_t new_factor = (new_cap>>1) + (new_cap>>2);

    map_node_t *new_bks = stalloc(new_cap, map_node_t);
    if (!new_bks) {
        log_error("map_realloc: stalloc");
        exit(2);
    }

    len_t cnt = 0;
    for (len_t i = 0; i < map->cap; ++i) {
        if (!map->bks[i]) continue;
        map_insert(map->bks[i], new_bks, new_capm1);
        if (++cnt >= map->len) break;
    }
    map->bks = new_bks;
    map->cap = new_cap;
    map->capm1 = new_capm1;
    map->factor = new_factor;
}

int map_set(map_t map, str_t key, str_t value)
{
    if (map->len >= map->factor) {
        map_realloc(map);
    }

    map_node_t p = map_getref(map, key);
    if (p) {
        p->value = value;
        return 0;
    }

    map_insert_kv(key, value, map->bks, map->capm1);
    ++map->len;
    return 0;
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
