#pragma once

#include "str.h"

// given NULL, use DEFAULT_LOG_FILE in config.h
void log_init(str_t log_file);

// write log to stdout and file, with prefix INFO
void log_info(str_t fmt, ...);

// write log to stdout and file, with prefix WARN
void log_warn(str_t fmt, ...);

// write log to stderr and file, with prefix ERROR
void log_error(str_t fmt, ...);

// write log to stderr and file, with prefix DEBUG
void log_debug(str_t fmt, ...);
