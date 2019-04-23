#pragma once

#include "str.h"

// given NULL, use DEFAULT_LOG_FILE in config.h
void log_init(const str_t *log_file);

// write log to stdout and file, with prefix INFO
void log_info(const char *fmt, ...);

// write log to stdout and file, with prefix WARN
void log_warn(const char *fmt, ...);

// write log to stderr and file, with prefix ERROR
void log_error(const char *fmt, ...);

// write log to stderr and file, with prefix DEBUG
void log_debug(const char *fmt, ...);
