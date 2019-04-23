#pragma once

#include "types.h"
#include "array.h"
#include <stdbool.h>

typedef struct {
    len_t len;
    char *s;
} str_t;

str_t *str_new(const char *s);

str_t *str_new_n(const char *s, len_t len);

// clone s, use this to get a non-const pointer
#define str_clone(str) str_new(str->s)

// trim space(\r\n\t) from both sides,
// return the trimed str_t, leave the original one unchanged (immutable)
str_t *str_trim(const str_t *str);

// trim space(\r\n\t) from left side
// return the trimed str_t, leave the original one unchanged (immutable)
str_t *str_trim_left(const str_t *str);

// trim space(\r\n\t) from right side
// return the trimed str_t, leave the original one unchanged (immutable)
str_t *str_trim_right(const str_t *str);

// return true when s1 has the same string as s2
bool str_equal(const str_t *s1, const str_t *s2);

// wrapping for strcmp
int str_cmp(const str_t *s1, const str_t *s2);

// split at character sep
array_t *str_split(const str_t *s, char sep);

// split at most n piece, if n == -1, split as much as it could
array_t *str_split_n(const str_t *s, char sep, int n);
