#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "map.h"

ehp_map_t *ehp_map_new(size_t nbuckets)
{
    ehp_map_t *map = (ehp_map_t*)malloc(sizeof(ehp_map_t));
    if (map == NULL) {
        return NULL;
    }
    map->buckets = (ehp_map_node_t**)calloc(nbuckets, sizeof(ehp_map_node_t*));
    if (map->buckets == NULL) {
        return NULL;
    }
    map->nbuckets = nbuckets;
    return map;
}

void ehp_map_free(ehp_map_t *map)
{
    int nbuckets = map->nbuckets;
    ehp_map_node_t **buckets = map->buckets;
    ehp_map_node_t *p, *next;

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

static ehp_map_hash_t ehp_map_hash(const ehp_map_t *map, const char *s)
{
    ehp_map_hash_t hashval = 0;
    for (; *s; ++s)
        hashval = *s + 31 * hashval;
    return hashval % map->nbuckets;
}

static ehp_map_node_t *ehp_map_new_node(const ehp_map_t *map, const char *key, const char *value)
{
    int key_len = strlen(key) + 1;
    int value_len = strlen(value) + 1;
    ehp_map_node_t *node = (ehp_map_node_t*)malloc(sizeof(ehp_map_node_t));
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
    node->hash = ehp_map_hash(map, key);
    strncpy(node->key, key, key_len);
    strncpy(node->value, value, value_len);
    return node;
}

static ehp_map_node_t *map_getref(const ehp_map_t *map, const char *key)
{
    ehp_map_hash_t hash = ehp_map_hash(map, key);
    ehp_map_node_t *p = map->buckets[hash];

    while (p) {
        if (p->hash == hash && !strcmp(p->key, key)) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

int ehp_map_set(ehp_map_t *map, const char *key, const char *value)
{
    if (key == NULL || *key == '\0' || value == NULL) {
        return -1;
    }
    ehp_map_node_t *p = map_getref(map, key);
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
    p = ehp_map_new_node(map, key, value);
    p->next = map->buckets[p->hash];
    map->buckets[p->hash] = p;
    return 0;
}

char *ehp_map_get(const ehp_map_t *map, const char *key)
{
    ehp_map_node_t *p = map_getref(map, key);
    if (p != NULL) {
        return p->value;
    }
    return NULL;
}
