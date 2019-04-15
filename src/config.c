#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "config.h"
#include "map.h"
#include "array.h"
#include "util.h"
#include "log.h"

// max line length of config file
#define EHP_CONFIG_MAXLINE 8192

static ehp_config_t *ehp_config_new(void)
{
    ehp_config_t *config = (ehp_config_t*)malloc(sizeof(ehp_config_t));
    if (config == NULL) {
        return NULL;
    }

    config->props = map_new(32);
    if (config->props == NULL) {
        return NULL;
    }

    config->routes = array_new();
    if (config->routes == NULL) {
        return NULL;
    }
    return config;
}

ehp_config_t *ehp_config_read_from(const char *filename)
{
    char buf[EHP_CONFIG_MAXLINE];

    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        return NULL;
    }

    ehp_config_t *config = ehp_config_new();
    if (config == NULL) {
        return NULL;
    }

    // TODO: fetch this part as single function
    while (fgets(buf, sizeof(buf), f) != NULL) {
        char *s = ehp_util_trim(buf);

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
                ehp_log_error("config: malloc");
                exit(1);
            }
            strncpy(rrule, value, len);
            if (array_push_back(config->routes, (void*)rrule) < 0) {
                ehp_log_error("config: array append");
                exit(1);
            }
            continue;
        }

        // a kv property
        if (map_set(config->props, key, value) < 0) {
            ehp_log_error("config: map set");
            exit(1);
            continue;
        }
    }
    fclose(f);
    return config;
}
