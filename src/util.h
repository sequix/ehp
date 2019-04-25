#pragma once

#include <stdlib.h>

// set non-blocking
int file_set_nonblock(int fd);

// read file content blockingly
char *file_read(const char *path, size_t *len);
