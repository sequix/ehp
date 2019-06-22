#include "log.h"
#include "config.h"
#include "str.h"
#include "array.h"
#include <stdio.h>

// the global config
config_t config = NULL;

int config_init(str_t filename)
{
    config = (config_t)map_new(32);
    if (config == NULL) {
        log_odebug("config: map_new");
        exit(1);
    }

    FILE *f = fopen(str_tocc(filename), "r");
    if (f == NULL) {
        log_odebug("config: %r", filename);
        exit(1);
    }

    str_t config_content = str_from_file(f);
    array_t config_lines = str_split(config_content, '\n');
    len_t nlines = array_len(config_lines) - 2;

    for (len_t i = 0; i < nlines; ++i) {
        str_t line = array_get_str(config_lines, i);
        if (str_at(line, 0) == '#') continue;
        array_t kv = str_nsplit(line, ':', 2);
        if (kv == NULL) continue;
        str_t key = array_get_str(kv, 0);
        str_t value = array_get_str(kv, 1);
        value = str_sub(value, 1, str_len(value));
        map_set((map_t)config, key, value);
    }
    log_odebug("config: init success");
    return 0;
}
