#include "mem.h"
#include "net.h"
#include "config.h"
#include "str.h"
#include "http.h"
#include "file.h"
#include "regexp.h"
#include "log.h"
#include "map.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>

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

// 404 page
http_rsp_t http_rsp_404 = NULL;

http_req_t http_req_new(char *raw, len_t len, int fd)
{
    http_req_t req = talloc(http_req_st);
    if (req == NULL) {
        log_error("http: talloc http_req_st");
        return NULL;
    }

    char *p = strchr(raw, '\r');
    if (!p) {
        log_error("http: invalid http message");
        return req;
    }
    *p = '\0';
    regexp_submatch_st mat[4];

    if (regexp_match(regexp_http_req_first, raw, 4, mat) != 0) {
        log_error("http: regexp_match");
        return NULL;
    }
    *p = '\r';

    req->method = str_nfrom(raw, 0, mat[1].rm_eo);
    if (req->method == NULL) {
        log_error("http: parse error: no method");
        return NULL;
    }

    str_t addr = str_nfrom(raw, mat[2].rm_so, mat[2].rm_eo - mat[2].rm_so);
    req->addr = url_parse(str_tocc(addr));
    if (req->addr == NULL) {
        log_error("http: parse error: no url");
        return NULL;
    }

    req->version = str_nfrom(raw, mat[3].rm_so, mat[3].rm_eo - mat[3].rm_so);
    if (req->version == NULL) {
        log_error("http: parse error: no version");
        return NULL;
    }

    req->remote_addr = socket_peer_addr(fd);
    req->raw = raw;
    req->raw_len = len;

    // headers
    req->headers = (http_header_t)map_new();
    if (req->headers == NULL) {
        log_error("http: map_new() headers");
        return NULL;
    }

    char *line = next_line(raw);

    for ( ;line && line[0] != '\r'; line = next_line(line)) {
        char *pcolon = strchr(line, ':');
        char *pcr = strchr(line, '\r');
        map_set(req->headers, str_nfrom(line, 0, pcolon - line),
                str_nfrom(pcolon+2, 0, pcr - pcolon - 2));
    }
    return req;
}

static http_rsp_t http_rsp_new(void)
{
    http_rsp_t rsp = talloc(http_rsp_st);
    if (rsp == NULL) {
        log_error("http_rsp: alloc");
        return NULL;
    }

    rsp->headers = http_header_new();
    if (rsp->headers == NULL) {
        log_error("http_rsp: alloc header");
        return NULL;
    }
    rsp->version = HTTP_VERSION_1_1;
    return rsp;
}

void http_init()
{
    http_rsp_404 = http_rsp_from_file("/404.html");
    http_rsp_404->status = 404;
}

http_rsp_t http_rsp_from_file(str_t filepath)
{
    http_rsp_t rsp = http_rsp_new();
    if (!rsp) {
        return rsp;
    }

    if (str_at(filepath, str_len(filepath)-1) == '/') {
        filepath = str_cat(filepath, "index.html");
    }

    str_t path = str_cat(config_get(CONFIG_WWW_ROOT), filepath);

    int fd = open(str_tocc(path), O_RDONLY | O_EXCL);
    if (fd < 0) {
        log_debug("open %r", path);
        return http_rsp_404;
    }

    struct stat st;
    if (fstat(fd, &st) < 0) {
        log_debug("failed to stat '%r'", path);
        return http_rsp_404;
    }
    rsp->len = st.st_size;
    rsp->status = 200;
    rsp->bodyfd = fd;
    return rsp;
}


int http_rsp_write(http_rsp_t rsp, int fd)
{
    char buf[4096];

    int len = sprintf(buf, "%s %d %s\r\n", rsp->version, rsp->status,
        http_get_status_msg(rsp->status));

    len += sprintf(buf+len, "Content-Length: %d\r\n\r\n", rsp->len);

    if (write(fd, buf, len) != len) {
        log_error("write response upper part failed");
        return -1;
    }

    if (sendfile(fd, rsp->bodyfd, NULL, rsp->len) != rsp->len) {
        log_error("sendfile response body failed");
        return -1;
    }
    close(rsp->bodyfd);
    return 0;
}
