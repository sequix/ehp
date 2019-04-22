#pragma once

#include <stdlib.h>

#define talloc(type) ((type*)malloc(sizeof(type)))

// trim space from both sides
char *str_trim(char *s);

// trim space from left side
char *str_trim_left(char *s);

// trim space from right side
char *str_trim_right(char *s);

// copy string to heap, return pointer to the copied string
char *str_copy(const char *s);

// copy most n chars to heap, return pointer to the copied string
char *str_copy_n(const char *s, size_t n);

