#include "log.h"
#include "str.h"
#include "ptime.h"
#include "config.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>

static int log_fd = -1;

void log_init(str_t log_file)
{
    if (log_file == NULL) {
        log_file = DEFAULT_LOG_FILE;
    }
    log_fd = open(str_tocc(log_file), O_APPEND | O_CREAT | O_WRONLY, 0644);
    if (log_fd < 0) {
        perror(str_tocc(log_file));
        exit(1);
    }
    log_info("log: init success");
}

static str_t log_generate(str_t prefix, int the_errno, str_t fmt, va_list args)
{
    char buf[64];

    ptime_t pt = ptime();
    str_t l_time = STR_EMPTY;
    str_sprintf(&l_time, "%04d/%02d/%02d %02d:%02d:%02d.%03ld ", pt->year,
        pt->month, pt->day, pt->hour, pt->minute, pt->second, pt->millisecond);

    str_t l_pid = STR_EMPTY;
    str_sprintf(&l_pid, "%d ", getpid());

    str_t l_prefix = STR_EMPTY;
    if (str_len(prefix) > 0) {
        l_prefix = str_catc(prefix, ' ');
    }

    str_t l_errno = STR_EMPTY;
    if (the_errno != 0) {
        strerror_r(the_errno, buf, sizeof(buf));
        str_sprintf(&l_errno, "%s: ", buf);
    }

    str_t l_msg = STR_EMPTY;
    str_vsprintf(&l_msg, fmt, args);
    l_msg = str_catc(l_msg, '\n');
    return str_catn(5, l_time, l_pid, l_prefix, l_errno, l_msg);
}

static int log_to_file(str_t msg)
{
    int len = str_len(msg);

    if (write(log_fd, str_tocc(msg), len) != len) {
        perror("log to file");
        return -1;
    }
    return 0;
}

static int log_to_stdout(str_t msg)
{
    int len = str_len(msg);

    if (write(STDOUT_FILENO, str_tocc(msg), len) != len) {
        perror("log to stdout");
        return -1;
    }
    return 0;
}

static int log_to_stderr(str_t msg)
{
    int len = str_len(msg);

    if (write(STDERR_FILENO, str_tocc(msg), len) != len) {
        perror("log to stderr");
        return -1;
    }
    return 0;
}

void log_info(str_t fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    str_t log = log_generate("INFO", 0, fmt, args);
    log_to_stdout(log);
    log_to_file(log);
    va_end(args);
}

void log_warn(str_t fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    str_t log = log_generate("WARN", 0, fmt, args);
    log_to_stdout(log);
    log_to_file(log);
    va_end(args);
}

void log_error(str_t fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    str_t log = log_generate("ERROR", errno, fmt, args);
    log_to_stderr(log);
    log_to_file(log);
    va_end(args);
}

void log_debug(str_t fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    str_t log = log_generate("DEBUG", errno, fmt, args);
    log_to_stdout(log);
    log_to_file(log);
    va_end(args);
}
