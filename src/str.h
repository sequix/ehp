#pragma once

#include "types.h"
#include "array.h"
#include <gc.h>
#include <gc/cord.h>
#include <stdint.h>

// str_t are immutable character strings,
// it is actually a const char*, but NEVER treat as a const char*
// if you want use a char* or const char*, use str_toc, str_tocc to convert
typedef CORD str_t;

// empty string, it is actuall 0 or NULL
// NOTICE: Never "" for str_t, use STR_EMPTY instead
#define STR_EMPTY CORD_EMPTY

// "not found" return from str_chr, str_rchr, str_str
#define STR_NOT_FOUND CORD_NOT_FOUND 

// make a str_t from a char* or const char*
#define str_from(s) CORD_from_char_star(s)

// make a str_t with at most n chars, starts at i
str_t str_nfrom(const char *s, int i, int n);

// convert str_t to char*
#define str_toc(s) CORD_to_char_star(s)

// convert str_t to const char*
#define str_tocc(s) CORD_to_const_char_star(s)

// convert int64_t to str_t
str_t str_from_int64(int64_t x);

// convert uint64_t to str_t
str_t str_from_uint64(int64_t x);

// return length of a str_t
// WARN: str_len("") will return abitrary values
#define str_len(s) CORD_len(s)

// concatenation of two str_t
#define str_cat(s1,s2) CORD_cat((s1), (s2))

// concatenation of s1 and s2's left most n chars
#define str_ncat(s1,s2,n) CORD_cat_char_star(s1,s2,n)

// concatenation of s and character c
#define str_catc(s,c) CORD_cat_char(s,c)

// Concatenate n str_t
#define str_catn(n,...) CORD_catn(n,__VA_ARGS__)

// substring of s from i with at most n chars
#define str_sub(s,i,n) CORD_substr(s,i,n)

// return ith(i is int) char (slower than str_pat)
#define str_at(s,i) CORD_fetch(s,i)

// return true if x = y, false if otherwise
#define str_eq(x,y) (!CORD_cmp(x,y))

// return < 0, 0, or > 0, depending on whether x < y, x = y, x > y
#define str_cmp(x,y) CORD_cmp(x,y)

// a generalization of string comparision
#define str_ncmp(x,xi,y,yi,n) CORD_ncmp(x,xi,y,yi,n)

// a analogus of strchr, return index rather pointer
#define str_chr(s,i,c) CORD_chr(s,i,c)

// a analogus of strchr, return the last index rather pointer
#define str_rchr(s,i,c) CORD_rchr(s,i,c)

// printf-family functions
#define str_printf(...)  CORD_printf(__VA_ARGS__)
#define str_sprintf(...) CORD_sprintf(__VA_ARGS__)
#define str_fprintf(...) CORD_fprintf(__VA_ARGS__)

#define str_vprintf(...)  CORD_vprintf(__VA_ARGS__)
#define str_vsprintf(...) CORD_vsprintf(__VA_ARGS__)
#define str_vfprintf(...) CORD_vfprintf(__VA_ARGS__)

// internal index type
typedef CORD_pos str_i_t;

// get the char pi pointing to
#define str_pat(pi) CORD_pos_fetch(pi)

// return int index from str_i_t
#define str_i_toidx(pi) CORD_pos_to_index(pi)

// set internal index pi to the ith char
#define str_i_set(s,pi,i) CORD_set_pos(pi,s,i)

// return true if pi is still pointing some char
#define str_i_test(pi) CORD_pos_valid(pi)

// move pi to the next char
#define str_i_next(pi) CORD_next(pi)

// move pi to the previous char
#define str_i_prev(pi) CORD_prev(pi)

// iterator pattern
#define str_for(s,pi) \
    for (str_i_set(s,pi,0); str_i_test(pi); str_i_next(pi))
#define str_rfor(s,pi) \
    for (str_i_set(s,pi,str_len(s)-1); str_i_test(pi); str_i_prev(pi))

// accept a FILE*, return the content as str_t, call fclose automatically
#define str_from_file(f) CORD_from_file(f)

// Things I didn't wrap for now:
//   1.file-related functions, like CORD_from_file
//   2.iteration with function
// The following is what CORD not provided.

// return true when str are STR_EMPTY, NULL, 0, "" and str_len(str) == 0
#define str_isempty(s) ((s) == STR_EMPTY)

// trim space( \r\n\t) from both sides, return the trimed
str_t str_trim(str_t str);

// trim space( \r\n\t) from left side, return the trimed
str_t str_ltrim(str_t str);

// trim space( \r\n\t) from right side, return the trimed
str_t str_rtrim(str_t str);

// return a hash value of s
hash_t str_hash(str_t str);

// split at character sep
#define str_split(s,sep) str_nsplit(s,sep,-1)

// split at most n piece, if n == -1, split as much as it could
array_t str_nsplit(str_t s, char sep, int n);
