#include "http.h"
#include "util.h"
#include "regexp.h"
#include "log.h"
#include "map.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>

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

http_rsp_t *http_rsp_new(void)
{
    http_rsp_t *rsp = talloc(http_rsp_t);
    if (rsp == NULL) {
        log_error("malloc");
        return NULL;
    }

    rsp->headers = http_header_new();
    if (rsp->headers == NULL) {
        log_error("malloc");
        return NULL;
    }
    return rsp;
}

void http_rsp_free(http_rsp_t *rsp)
{
    http_header_free(rsp->headers);
    free(rsp);
}

int http_rsp_write(http_rsp_t *rsp, int fd)
{
    char buf[1024];

    sprintf(buf, "%s %d %s\r\n", rsp->version, rsp->status, http_get_status_msg(rsp->status));
    int len = strlen(buf);
    if (write(fd, buf, len) != len) {
        log_error("write() response head line");
        return -1;
    }

    sprintf(buf, "Content-Length: %d\r\n\r\n", rsp->len);
    len = strlen(buf);
    if (write(fd, buf, len) != len) {
        log_error("write() response Content length header");
        return -1;
    }

    if (write(fd, rsp->body, rsp->len) != rsp->len) {
        log_error("write() respones body");
        return -1;
    }
    return 0;
}
