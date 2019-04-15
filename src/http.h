#pragma once

#include "map.h"

typedef map_t http_header_t;
int   http_header_insert(http_header_t *header, const char *line);
int   http_header_insert_batch(http_header_t *header, const char *str);
int   http_header_insert_from(http_header_t *header, int fd);
char* http_header_get(http_header_t *header, const char *key);
int   http_header_set(http_header_t *header, const char *key, const char *value);

typedef unsigned short http_status_code_t;
const char *http_get_status_msg(http_status_code_t code);

typedef const char *http_method_t;
extern http_method_t HTTP_METHOD_GET;
extern http_method_t HTTP_METHOD_POST;
extern http_method_t HTTP_METHOD_PUT;
extern http_method_t HTTP_METHOD_DELETE;
extern http_method_t HTTP_METHOD_PATCH;
extern http_method_t HTTP_METHOD_HEAD;
extern http_method_t HTTP_METHOD_CONNECT;
extern http_method_t HTTP_METHOD_TRACE;
extern http_method_t HTTP_METHOD_OPTIONS;
