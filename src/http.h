#pragma once

#include "url.h"
#include "map.h"
#include <sys/types.h>

typedef map_t http_header_t;
#define http_header_new() (http_header_t)map_new()
int   http_header_insert(http_header_t *header, const char *line);
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

typedef const char*   http_version_t;
extern http_version_t HTTP_VERSION_1_0;
extern http_version_t HTTP_VERSION_1_1;
extern http_version_t HTTP_VERSION_2;

typedef struct {
    str_t          remote_addr;
    http_method_t  method;
    url_t          addr;
    http_version_t version;
    http_header_t  headers;
    len_t          raw_len;
    char*          raw;
} http_req_st;

typedef http_req_st* http_req_t;

http_req_t http_req_new(char *raw, len_t len, int fd);

void http_req_free(http_req_t req);

// for now, only Keep-Alive, which means always with Content-Length
typedef struct {
    http_version_t     version;
    http_status_code_t status;
    http_header_t      headers;
    len_t              len;
    int                bodyfd;
} http_rsp_st;

typedef http_rsp_st* http_rsp_t;

http_rsp_t http_rsp_from_file(str_t filepath);

int http_rsp_write(http_rsp_t rsp, int fd);

char* http_rsp_text(http_rsp_t rsp);

typedef int (*http_handler_t)(const http_req_t request, http_rsp_t response);

void http_init();
