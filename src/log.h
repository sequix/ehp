#pragma once

// given NULL, use EHP_DEFAULT_LOG_FILE
void ehp_log_init(const char *log_file);

// write log to stdout and file, with prefix INFO
void ehp_log_info(const char *fmt, ...);

// write log to stdout and file, with prefix WARN
void ehp_log_warn(const char *fmt, ...);

// write log to stderr and file, with prefix ERROR
void ehp_log_error(const char *fmt, ...);

// write log to stderr and file, with prefix DEBUG
void ehp_log_debug(const char *fmt, ...);
