#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "config.h"
#include "map.h"
#include "array.h"
#include "util.h"
#include "log.h"

// max line length of config file
#define CONFIG_MAXLINE 8192

// the global config
config_t *config = NULL;

static config_t *config_new(void)
{
    config_t *config = talloc(config_t);
    if (config == NULL) {
        return NULL;
    }

    config->props = map_new(16);
    if (config->props == NULL) {
        return NULL;
    }

    config->routes = array_new();
    if (config->routes == NULL) {
        return NULL;
    }
    return config;
}

config_t *config_init(const char *filename)
{
    char buf[CONFIG_MAXLINE];

    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        return NULL;
    }

    config = config_new();
    if (config == NULL) {
        return NULL;
    }

    // TODO: fetch this part as single function
    while (fgets(buf, sizeof(buf), f) != NULL) {
        char *s = str_trim(buf);

        // comment line
        if (*s == '#') {
            continue;
        }

        // config line format <key>: <value>
        char *key = s;
        char *value = strchr(s, ':');
        *value = '\0';
        value += 2;

        // TODO: config error check
        
        // a routing rule
        if (!strcmp(key, "route")) {
            int len = strlen(value) + 1;
            char *rrule = (char*)malloc(len);
            if (rrule == NULL) {
                log_error("config: malloc");
                exit(1);
            }
            strncpy(rrule, value, len);
            if (array_push_back(config->routes, (void*)rrule) < 0) {
                log_error("config: array append");
                exit(1);
            }
            continue;
        }

        // a kv property
        if (map_set_c(config->props, key, value) < 0) {
            log_error("config: map set");
            exit(1);
            continue;
        }
    }
    fclose(f);
    return config;
}

int config_get_int(const char *key)
{
    str_t *s = map_get_c(config->props, key);
    if (s == NULL) {
        log_warn("config_get_int_c: no such config key '%s'", key);
        return 0;
    }
    return atoi(s->s);
}
