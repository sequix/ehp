#pragma once

#include <regex.h>

typedef regex_t regexp_st;
typedef regexp_st* regexp_t;
typedef regmatch_t regexp_submatch_st;
typedef regexp_submatch_st* regexp_submatch_t;

// see url.h
#define REGEXP_PTRN_URL "^\\(/[^?#]*\\)\\(?[^#]*\\)\\?\\(#.*\\)\\?$"
extern regexp_t regexp_url;

// first line of http request: '<method> <path> <version>'
#define REGEXP_PTRN_HTTP_REQ_FIRST "^\\([A-Z]*\\) \\([^ ]*\\) \\(HTTP/[0-9.]*\\)"
extern regexp_t regexp_http_req_first;

// init all the regulary expression ehp needs
int regexp_init(void);

regexp_t regexp_new(char *pattern);

#define regexp_free(reg) free(reg)

// return 0 if matches, non-zero if not
int regexp_match(regexp_t reg, const char *text, int nsubs,
    regexp_submatch_t subs);
