#pragma once

// TODO: remove argtable, implement yourself
#include "argtable3.h"

#define args_num 7
extern  struct   arg_lit  *args_help;
extern  struct   arg_lit  *args_version;
extern  struct   arg_lit  *args_verbose;
extern  struct   arg_lit  *args_daemon;
extern  struct   arg_file *args_config_file;
extern  struct   arg_file *args_log_file;
extern  struct   arg_end  *args_err;

int args_handle(int argc, char *argv[]);
