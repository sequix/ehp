#include "http.h"
#include "util.h"
#include "regexp.h"
#include "log.h"

static char *next_line(char *last_line)
{
    int i = 0;
    while (last_line[i] != '\n') {
        ++i;
    }
    if (last_line[i+1] == '\0') {
        return NULL;
    }
    return &last_line[i+1];
}

http_req_t *http_req_new(char *raw, size_t len, int clientfd)
{
    http_req_t *req = talloc(http_req_t);
    if (req == NULL) {
        return NULL;
    }

    req->headers = (http_header_t*)map_new(64);
    if (req->headers == NULL) {
        return NULL;
    }

    char *line = raw;
    regexp_submatch_t mat[4];

    if (regexp_match(regexp_http_req_first, line, 4, mat) != 0) {
        return NULL;
    }

    char *method = line;
    line[mat[1].rm_eo] = '\0';
    req->method = str_copy(method);
    if (req->method == NULL) {
        return NULL;
    }

    char *addr = &line[mat[2].rm_so];
    line[mat[2].rm_eo] = '\0';
    req->addr = url_parse(addr);
    if (req->addr == NULL) {
        return NULL;
    }

    char *version = &line[mat[3].rm_so];
    line[mat[3].rm_eo] = '\0';
    req->version = str_copy(version);
    if (req->version == NULL) {
        return NULL;
    }

    req->raw = raw;
    req->raw_len = len;
    req->clientfd = clientfd;

    // TODO: header
    //while (1) {
    //    char *nl = next_line(line);
    //    if (*nl == '\0' || *nl == '\r') {
    //        break;
    //    }
    //    http_header_insert(req->headers, line);
    //}

    // TODO: body
    //req->body_len = len;
    //req->body = body;
    return req;
}
