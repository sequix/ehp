#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "map.h"

map_t *map_new(size_t nbuckets)
{
    map_t *map = (map_t*)malloc(sizeof(map_t));
    if (map == NULL) {
        return NULL;
    }
    map->buckets = (map_node_t**)calloc(nbuckets, sizeof(map_node_t*));
    if (map->buckets == NULL) {
        return NULL;
    }
    map->nbuckets = nbuckets;
    return map;
}

void map_free(map_t *map)
{
    int nbuckets = map->nbuckets;
    map_node_t **buckets = map->buckets;
    map_node_t *p, *next;

    for (int i = 0; i < nbuckets; ++i) {
        if (buckets[i] == NULL) {
            continue;
        }
        for (p = buckets[i]; p; p = next) {
            next = p->next;
            printf("%s: %s\n", p->key, p->value);
            free(p->key);
            free(p->value);
            free(p);
        }
    }
}

static map_hash_t map_hash(const map_t *map, const char *s)
{
    map_hash_t hashval = 0;
    for (; *s; ++s)
        hashval = *s + 31 * hashval;
    return hashval % map->nbuckets;
}

static map_node_t *map_new_node(const map_t *map, const char *key, const char *value)
{
    int key_len = strlen(key) + 1;
    int value_len = strlen(value) + 1;
    map_node_t *node = (map_node_t*)malloc(sizeof(map_node_t));
    if (node == NULL) {
        perror("malloc for node");
        return NULL;
    }
    node->key = (char*)malloc(key_len);
    if (node->key == NULL) {
        perror("malloc for node->key");
        free(node);
        return NULL;
    }
    node->value = (char*)malloc(value_len);
    if (node->key == NULL) {
        perror("malloc for node->value");
        free(node->key);
        free(node);
        return NULL;
    }
    node->hash = map_hash(map, key);
    strncpy(node->key, key, key_len);
    strncpy(node->value, value, value_len);
    return node;
}

static map_node_t *map_getref(const map_t *map, const char *key)
{
    map_hash_t hash = map_hash(map, key);
    map_node_t *p = map->buckets[hash];

    while (p) {
        if (p->hash == hash && !strcmp(p->key, key)) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

int map_set(map_t *map, const char *key, const char *value)
{
    if (key == NULL || *key == '\0' || value == NULL) {
        return -1;
    }
    map_node_t *p = map_getref(map, key);
    if (p != NULL) {
        int new_value_len = strlen(value) + 1;
        int old_value_len = strlen(p->value) + 1; 
        if (old_value_len >= new_value_len) {
            strncpy(p->value, value, new_value_len);
            return 0;
        }
        p->value = (char*)realloc(p->value, new_value_len);
        if (p->value == NULL) {
            return -1;
        }
        strncpy(p->value, value, new_value_len);
        return 0;
    }
    p = map_new_node(map, key, value);
    p->next = map->buckets[p->hash];
    map->buckets[p->hash] = p;
    return 0;
}

char *map_get(const map_t *map, const char *key)
{
    map_node_t *p = map_getref(map, key);
    if (p != NULL) {
        return p->value;
    }
    return NULL;
}
