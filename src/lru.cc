#include "lru.h"
#include "mem.h"
#include "log.h"
#include <stdlib.h>
#include <string.h>

#define LRU_HASH_INIT_SIZE 64

// TODO: reuse map_t

lru_t lru_new(len_t cap)
{
    lru_t lru = talloc(lru_st);
    if (!lru) {
        log_error("lru_new: talloc");
        exit(1);
    }

    lru->hash_bks = stalloc(LRU_HASH_INIT_SIZE, lru_node_t);
    if (!lru->hash_bks) {
        log_error("lru_new: stalloc");
        exit(1);
    }
    lru->cap = cap;
    lru->hash_cap = LRU_HASH_INIT_SIZE;
    lru->hash_capm1 = LRU_HASH_INIT_SIZE - 1;
    lru->hash_factor = (LRU_HASH_INIT_SIZE>>1) + (LRU_HASH_INIT_SIZE>>2);
    return lru;
}

static lru_node_t lru_node_new(str_t key, str_t value)
{
    lru_node_t node = talloc(lru_node_st);
    if (!node) {
        log_error("lru_node_new: talloc");
        exit(1);
    }
    node->key = key;
    node->value = value;
    node->len = str_len(value);
    return node;
}

static int lru_hash_set_actual(lru_t lru, lru_node_t node)
{
    hash_t hash = str_hash(node->key) & lru->hash_capm1;
    hash_t i = hash;
    do {
        if (!lru->hash_bks[i] || str_eq(node->key, lru->hash_bks[i]->key)) {
            lru->hash_bks[i] = node;
            return 0;
        }
        i = (i + 1) & lru->hash_capm1;
    } while (i != hash);
    return -1;
}

static int lru_hash_realloc(lru_t lru)
{
    len_t old_cap = lru->hash_cap;
    len_t new_cap = old_cap << 1;
    lru_node_t *new_bks = stalloc(new_cap, lru_node_t);
    if (!new_bks) {
        log_error("lru_hash_realloc: stalloc");
        exit(1);
    }

    len_t cnt = 0;
    lru_node_t *old_bks = lru->hash_bks;

    lru->hash_bks = new_bks;
    lru->hash_cap = new_cap;
    lru->hash_capm1 = new_cap - 1;
    lru->hash_factor = (new_cap>>1) + (new_cap>>2);

    for (len_t i = 0; i < old_cap; ++i) {
        if (!old_bks[i]) continue;
        if (lru_hash_set_actual(lru, old_bks[i]) < 0) {
            return -1;
        }
        if (++cnt >= lru->len) break;
    }
    return 0;
}

static int lru_hash_set(lru_t lru, lru_node_t node)
{
    if (lru->len >= lru->hash_cap) {
        if (lru_hash_realloc(lru) < 0) {
            return -1;
        }
    }
    return lru_hash_set_actual(lru, node);
}

static int lru_list_set(lru_t lru, lru_node_t node)
{
    if (lru->len == 0) {
        lru->list_head = node;
        lru->list_tail = node;
    } else {
        node->next = lru->list_head;
        node->prev = NULL;
        lru->list_head->prev = node;
        lru->list_head = node;
    }
    return 0;
}

static int lru_hash_index(lru_t lru, str_t key)
{
    lru_node_t *bks = lru->hash_bks;
    hash_t hash = str_hash(key) & lru->hash_capm1;
    hash_t i = hash;
    do {
        if (bks[i] && str_eq(key, bks[i]->key)) {
            return i;
        }
        i = (i + 1) & lru->hash_capm1;
    } while (i != hash);
    return -1;
}

static int lru_hash_release(lru_t lru, lru_node_t node)
{
    int i = lru_hash_index(lru, node->key);
    if (i < 0) {
        return -1;
    }
    lru->hash_bks[i] = NULL;
    return 0;
}

static int lru_release(lru_t lru, len_t need)
{
    len_t released = 0;
    lru_node_t p = NULL;

    for (p = lru->list_tail; p && released < need; p = p->prev) {
        lru_hash_release(lru, p);
        released += p->len;
        lru->len -= p->len;
    }

    if (!p) {
        lru->list_head = lru->list_tail = NULL;
    } else {
        lru->list_tail = p;
        p->next = NULL;
    }
    return 0;
}

int lru_set(lru_t lru, str_t key, str_t value)
{
    lru_node_t node = lru_node_new(key, value);
    if (!node) {
        return -1;
    }

    if (node->len + lru->len >= lru->cap) {
        // TODO: what if the file size bigger than the cache cap
        lru_release(lru, node->len);
    }

    if (lru_hash_set(lru, node) < 0) {
        return -2;
    }

    if (lru_list_set(lru, node) < 0) {
        return -3;
    }
    lru->len += node->len;
    return 0;
}

int lru_set_from_file(lru_t lru, str_t key, str_t filename)
{
    FILE *fp = fopen(str_tocc(filename), "r");
    if (!fp) {
        log_error("lru_set_from_file: fopen");
        return -1;
    }
    str_t value = str_from_file(fp);
    return lru_set(lru, key, value);
}

str_t lru_get(lru_t lru, str_t key)
{
    int i = lru_hash_index(lru, key);
    if (i < 0) {
       return STR_EMPTY;
    }
    return lru->hash_bks[i]->value;
}
