#include "file.h"
#include "log.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int file_set_nonblock(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        return -1;
    }
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

// read file content blockingly
char *file_read(const char *path, size_t *len)
{
    struct stat sb;

    if (stat(path, &sb) < 0) {
        log_error("stat %s", path);
        return NULL;
    }

    if ((sb.st_mode & S_IFMT) != S_IFREG) {
        log_error("expected %s a regular file", path);
        return NULL;
    }

    *len = sb.st_size;
    char *buf = (char*)malloc(*len);
    if (buf == NULL) {
        log_error("malloc");
        return NULL;
    }

    int fd = open(path, O_RDONLY, 0);
    if (fd < 0) {
        log_error("open()");
        return NULL;
    }

    if (read(fd, buf, *len) < 0) {
        log_error("read()");
        return NULL;
    }
    return buf;
}
