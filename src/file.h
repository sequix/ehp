#pragma once

#include "str.h"
#include <stdlib.h>
#include <stdbool.h>

// set non-blocking
int file_set_nonblock(int fd);

// read file content blockingly
char *file_read(const char *path, size_t *len);

// return all the content in nonblocking fd
char *file_read_nonblock(int fd, int *len, bool *closed);
