#include "log.h"
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include "config.h"

// TODO: remove this length limit of log
// maximum length of each log
#define EHP_LOG_MAXLINE 8192

static int ehp_log_fd = -1;

void ehp_log_init(const char *log_file)
{
    if (log_file == NULL) {
        log_file = EHP_DEFAULT_LOG_FILE;
    }
    ehp_log_fd = open(log_file, O_APPEND | O_CREAT | O_WRONLY, 0644);
    if (ehp_log_fd < 0) {
        perror("log: init: open()");
        exit(1);
    }
    ehp_log_info("log: init success");
}

static void ehp_log_generate(char *s, const char *prefix, const char *fmt, va_list args)
{
    time_t now = time(0);
    struct tm t;
    localtime_r(&now, &t);

    sprintf(s, "%04d/%02d/%02d %02d:%02d:%02d", t.tm_year + 1900,
        t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
    s = s + strlen(s);

    if (prefix != NULL && *prefix != '\0') {
        sprintf(s, " %s ", prefix);
        s = s + strlen(s);
    }
    vsprintf(s, fmt, args);
    s = s + strlen(s);
    sprintf(s, "\n");
}

static int ehp_log_to_file(const char *msg)
{
    int len = strlen(msg);

    if (write(ehp_log_fd, msg, len) != len) {
        perror("log to file");
        return -1;
    }
    return 0;
}

static int ehp_log_to_stdout(const char *msg)
{
    int len = strlen(msg);

    if (write(STDOUT_FILENO, msg, len) != len) {
        perror("log to stdout");
        return -1;
    }
    return 0;
}

static int ehp_log_to_stderr(const char *msg)
{
    int len = strlen(msg);

    if (write(STDERR_FILENO, msg, len) != len) {
        perror("log to stderr");
        return -1;
    }
    return 0;
}

void ehp_log_info(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buf[EHP_LOG_MAXLINE];
    ehp_log_generate(buf, "INFO", fmt, args);
    ehp_log_to_stdout(buf);
    ehp_log_to_file(buf);
    va_end(args);
}

void ehp_log_warn(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buf[EHP_LOG_MAXLINE];
    ehp_log_generate(buf, "WARN", fmt, args);
    ehp_log_to_stdout(buf);
    ehp_log_to_file(buf);
    va_end(args);
}

void ehp_log_error(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buf[EHP_LOG_MAXLINE];
    ehp_log_generate(buf, "ERROR", fmt, args);
    ehp_log_to_stderr(buf);
    ehp_log_to_file(buf);
    va_end(args);
}

void ehp_log_debug(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buf[EHP_LOG_MAXLINE];
    ehp_log_generate(buf, "DEBUG", fmt, args);
    ehp_log_to_stderr(buf);
    ehp_log_to_file(buf);
    va_end(args);
}
