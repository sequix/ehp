#pragma once

#include "argtable3.h"

#define ehp_args_num 6
extern struct arg_lit *ehp_args_help;
extern struct arg_lit *ehp_args_version;
extern struct arg_lit *ehp_args_verbose;
extern struct arg_lit *ehp_args_daemon;
extern struct arg_file *ehp_args_config_file;
extern struct arg_end *ehp_args_err;

int ehp_args_handle(int argc, char *argv[]);
