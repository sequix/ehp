#include "str.h"
#include "log.h"
#include "mem.h"
#include "array.h"
#include <string.h>
#include <ctype.h>

str_t *str_new(const char *s)
{
    return str_new_n(s, strlen(s));
}

str_t *str_new_n(const char *s, len_t len)
{
    if (s == NULL) {
        log_warn("str_new_n: null pointer");
        return NULL;
    }
    str_t *str = talloc(str_t);
    if (str == NULL) {
        log_error("alloc str_t for \"%s\"", s);
        return NULL;
    }
    str->len = len;
    str->s = stalloc_a(len+1, char);
    if (str->s == NULL) {
        log_error("alloc str_t.s for \"%s\"", s);
        return NULL;
    }
    strncpy(str->s, s, len);
    str->s[len] = '\0';
    return str;
}

// trim space(\r\n\t) from both sides,
// return the trimed str_t, leave the original one unchanged (immutable)
str_t *str_trim(const str_t *str)
{
    if (str == NULL) {
        log_warn("str_trim: null pointer");
        return NULL;
    }

    char *s = str->s;
    int i = 0, j = 0;

    while (isspace(s[i])) {
        ++i;
    }

    for (j = str->len - 1; j > i && isspace(s[j]); --j) {
        ;
    }
    return str_new_n(s + i, j - i + 1);
}

// trim space(\r\n\t) from i side
// return the trimed str_t, leave the original one unchanged (immutable)
str_t *str_trim_left(const str_t *str)
{
    if (str == NULL) {
        log_warn("str_trim_left: null pointer");
        return NULL;
    }

    char *s = str->s;
    int i = 0;

    while (isspace(s[i])) {
        ++i;
    }
    return str_new_n(s + i, str->len - i);
}

// trim space(\r\n\t) from j side
// return the trimed str_t, leave the original one unchanged (immutable)
str_t *str_trim_right(const str_t *str)
{
    if (str == NULL) {
        log_warn("str_trim_right: null pointer");
        return NULL;
    }

    int j = 0;
    char *s = str->s;

    for (j = str->len - 1; j >= 0 && isspace(s[j]); --j) {
        ;
    }
    return str_new_n(s, j + 1);
}

// return true when s1 has the same string as s2
bool str_equal(const str_t *s1, const str_t *s2)
{
    if (s1 == NULL || s2 == NULL) {
        log_warn("str_equal: null pointer");
        return false;
    }
    if (s1->len != s2->len) {
        return false;
    }
    return !strcmp(s1->s, s2->s);
}

// wrapping for strcmp
int str_cmp(const str_t *s1, const str_t *s2)
{
    if (s1 == NULL || s2 == NULL) {
        log_warn("str_cmp: null pointer");
        return false;
    }
    return strcmp(s1->s, s2->s);
}

// split at character sep
array_t *str_split(const str_t *s, char sep)
{
    return str_split_n(s, sep, -1);
}

// split at most n piece, if n == -1, split as much as it could
array_t *str_split_n(const str_t *s, char sep, int n)
{
    if (s == NULL) {
        log_error("str_split_n: null pointer");
        return NULL;
    }

    if (n == -1) {
        n = s->len + 1;
    }

    array_t *arr = array_new();
    if (arr == NULL) {
        return NULL;
    }

    if (n == 0) {
        return arr;
    }

    if (n == 1) {
        array_push_back(arr, (void*)s);
        return arr;
    }

    char *cs = s->s;
    int i = 0, j = 0, len = s->len;
    for (; n > 1; --n) {
        while (j < len && cs[j] != sep) {
            ++j;
        }
        if (j == len) {
            break;
        }
        array_push_back(arr, (void*)str_new_n(cs+i, j-i));
        i = ++j;
    }
    array_push_back(arr, (void*)str_new_n(cs+i, len-i));
    return arr;
}
