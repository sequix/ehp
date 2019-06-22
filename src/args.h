#pragma once

#include "config.h"
#include "str.h"
#include <stdbool.h>

typedef struct {
    bool is_daemon;
    bool is_debug;
    str_t config;
} args_st;

typedef args_st* args_t;

// global arguments
extern args_t args;

// parse command arguments, if fail print help message and exit
void args_init(int argc, char *argv[]);
