#include "log.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <time.h>
#include "config.h"

// TODO: remove this length limit of log
// maximum length of each log
#define LOG_MAXLINE 8192

static int log_fd = -1;

void log_init(const char *log_file)
{
    if (log_file == NULL || *log_file == '\0') {
        log_file = DEFAULT_LOG_FILE;
    }
    log_fd = open(log_file, O_APPEND | O_CREAT | O_WRONLY, 0644);
    if (log_fd < 0) {
        perror("log: init: open()");
        exit(1);
    }
    log_info("log: init success");
}

static void log_generate(char *s, size_t cap, const char *prefix,
        bool with_errno, const char *fmt, va_list args)
{
    int len = 0;
    struct tm t;
    time_t now = time(0);
    localtime_r(&now, &t);

    snprintf(s+len, cap-len, "%04d/%02d/%02d %02d:%02d:%02d", t.tm_year + 1900,
        t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
    len += 19;


    if (prefix != NULL && *prefix != '\0') {
        snprintf(s+len, cap-len, " %s ", prefix);
        len += strlen(s+len);
    }

    if (with_errno) {
        strerror_r(errno, s+len, cap-len);
        len += strlen(s+len);
        snprintf(s+len, cap-len, ": ");
        len += 2;
    }

    vsnprintf(s+len, cap-len, fmt, args);
    len += strlen(s+len);
    snprintf(s+len, cap-len, "\n");
}

static int log_to_file(const char *msg)
{
    int len = strlen(msg);

    if (write(log_fd, msg, len) != len) {
        perror("log to file");
        return -1;
    }
    return 0;
}

static int log_to_stdout(const char *msg)
{
    int len = strlen(msg);

    if (write(STDOUT_FILENO, msg, len) != len) {
        perror("log to stdout");
        return -1;
    }
    return 0;
}

static int log_to_stderr(const char *msg)
{
    int len = strlen(msg);

    if (write(STDERR_FILENO, msg, len) != len) {
        perror("log to stderr");
        return -1;
    }
    return 0;
}

void log_info(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buf[LOG_MAXLINE];
    log_generate(buf, LOG_MAXLINE, "INFO", false, fmt, args);
    log_to_stdout(buf);
    log_to_file(buf);
    va_end(args);
}

void log_warn(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buf[LOG_MAXLINE];
    log_generate(buf, LOG_MAXLINE, "WARN", false, fmt, args);
    log_to_stdout(buf);
    log_to_file(buf);
    va_end(args);
}

void log_error(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buf[LOG_MAXLINE];
    log_generate(buf, LOG_MAXLINE, "ERROR", true, fmt, args);
    log_to_stderr(buf);
    log_to_file(buf);
    va_end(args);
}

void log_debug(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buf[LOG_MAXLINE];
    log_generate(buf, LOG_MAXLINE, "DEBUG", true, fmt, args);
    log_to_stderr(buf);
    log_to_file(buf);
    va_end(args);
}
