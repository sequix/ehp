#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "config.h"
#include "map.h"
#include "array.h"
#include "log.h"
#include "mem.h"

// the global config
config_t config = NULL;

static config_t config_new(void)
{
    config_t conf = talloc(config_st);
    if (conf == NULL) {
        return NULL;
    }

    conf->props = map_new(128);
    if (conf->props == NULL) {
        return NULL;
    }

    conf->routes = array_new(16);
    if (conf->routes == NULL) {
        return NULL;
    }
    return conf;
}

int config_init(str_t filename)
{
    FILE *f = fopen(str_tocc(filename), "r");
    if (f == NULL) {
        perror(filename);
        exit(1);
    }

    config = config_new();
    if (config == NULL) {
        return -2;
    }

    str_t config_content = str_from_file(f);
    array_t config_lines = str_split(config_content, '\n');

    for (len_t i = 0; i < array_len(config_lines); ++i) {
        str_t line = array_get_(str_t, config_lines, i);
        array_t kv = str_nsplit(line, ':', 2);
        if (kv == NULL) continue;

        str_t key = array_get_(str_t, kv, 0);
        str_t value = array_get_(str_t, kv, 1);
        value = str_sub(value, 1, str_len(value));

        // TODO: routing tree build
        // if (str_eq(key, "route")) { }

        map_set(config->props, key, value); 
    }
    return 0;
}
