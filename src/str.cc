#include "str.h"
#include "log.h"
#include "mem.h"
#include "types.h"
#include "config.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>

// trim space( \r\n\t) from both sides, return the trimed
str_t str_trim(str_t str)
{
    if (str_isempty(str)) {
        return STR_EMPTY;
    }

    str_i_t pi;
    int first_nonspace = -1;
    int last_nonspace = -1;

    str_for(str, pi) {
        if (!isspace(str_pat(pi))) {
            first_nonspace = str_i_toidx(pi);
            break;
        }
    }

    str_rfor(str, pi) {
        if (!isspace(str_pat(pi))) {
            last_nonspace = str_i_toidx(pi);
            break;
        }
    }
    return str_sub(str, first_nonspace, last_nonspace - first_nonspace + 1);
}

// make a str_t with at most n chars, starts at i
str_t str_nfrom(const char *s, int i, int n)
{
    char *buf = stalloc(n+1, char);
    if (!buf) {
        return NULL;
    }
    strncpy(buf, s+i, n);
    return str_from(buf);
}

// convert integer to str_t
str_t str_from_int64(int64_t x)
{
    char buf[32];
    sprintf(buf, "%ld", x);
    return str_from(buf);
}

// convert integer to str_t
str_t str_from_uint64(int64_t x)
{
    char buf[32];
    sprintf(buf, "%lu", x);
    return str_from(buf);
}

// trim space( \r\n\t) from left side, return the trimed
str_t str_ltrim(str_t str)
{
    if (str_isempty(str)) {
        return STR_EMPTY;
    }

    str_i_t pi;
    int first_nonspace = -1;

    str_for(str, pi) {
        if (!isspace(str_pat(pi))) {
            first_nonspace = str_i_toidx(pi);
            break;
        }
    }
    return str_sub(str, first_nonspace, str_len(str));
}

// trim space( \r\n\t) from right side, return the trimed
str_t str_rtrim(str_t str)
{
    if (str_isempty(str)) {
        return STR_EMPTY;
    }

    str_i_t pi;
    int last_nonspace = -1;

    str_rfor(str, pi) {
        if (!isspace(str_pat(pi))) {
            last_nonspace = str_i_toidx(pi);
            break;
        }
    }
    return str_sub(str, 0, last_nonspace + 1);
}

hash_t str_hash(str_t s)
{
    if (str_isempty(s)) {
        return STR_EMPTY;
    }
    str_i_t pi;
    hash_t hashval = HASH_SEED;
    str_for(s, pi) { 
        hashval = str_pat(pi) + 31 * hashval;
    }
    return hashval;
}

// split at most n piece, if n == -1, split as much as it could
array_t str_nsplit(str_t str, char sep, int n)
{
    if (str_isempty(str)) {
        return NULL;
    }

    int len = str_len(str);

    if (n == -1) {
        n = len + 1;
    }

    array_t arr = array_new(8);
    if (arr == NULL) {
        return NULL;
    }

    if (n == 0) {
        return arr;
    }

    if (n == 1) {
        array_push_back(arr, str);
        return arr;
    }

    str_i_t pi;
    str_i_set(str, pi, 0); 
    int i = 0, j = 0;

    for (; n > 1; --n) {
        for (; str_i_test(pi); str_i_next(pi)) {
            if (str_pat(pi) == sep) break;
            ++j;
        }
        if (j == len) {
            break;
        }
        array_push_back(arr, str_sub(str, i, j-i));
        str_i_next(pi);
        i = ++j;
    }
    array_push_back(arr, str_sub(str, i, len-i));
    return arr;
}
