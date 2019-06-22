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

//static char *next_line(char *last_line)
//{
//    int i = 0;
//    while (last_line[i] != '\n') {
//        ++i;
//    }
//    if (last_line[i+1] == '\0') {
//        return NULL;
//    }
//    return &last_line[i+1];
//}

http_req_t http_req_new(char *raw, len_t len, int fd)
{
    http_req_t req = talloc(http_req_st);
    if (req == NULL) {
        log_error("http: talloc http_req_st");
        return NULL;
    }

    req->headers = (http_header_t)map_new(64);
    if (req->headers == NULL) {
        log_error("http: map_new headers");
        return NULL;
    }

    char *line = raw;
    regexp_submatch_st mat[4];

    if (regexp_match(regexp_http_req_first, line, 4, mat) != 0) {
        log_error("http: regexp_match");
        return NULL;
    }

    char *method = line;
    line[mat[1].rm_eo] = '\0';
    req->method = str_from(method);
    if (req->method == NULL) {
        log_error("http: parse error: no method");
        return NULL;
    }

    char *addr = &line[mat[2].rm_so];
    line[mat[2].rm_eo] = '\0';
    req->addr = url_parse(addr);
    if (req->addr == NULL) {
        log_error("http: parse error: no url");
        return NULL;
    }

    char *version = &line[mat[3].rm_so];
    line[mat[3].rm_eo] = '\0';
    req->version = str_from(version);
    if (req->version == NULL) {
        log_error("http: parse error: no version");
        return NULL;
    }

    req->remote_addr = socket_peer_addr(fd);
    req->raw = raw;
    req->raw_len = len;

    // TODO: the rest content: header, body
    return req;
}

http_rsp_t http_rsp_from_file(str_t filepath)
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

    if (str_at(filepath, str_len(filepath)-1) == '/') {
        filepath = str_cat(filepath, "index.html");
    }

    str_t path = str_cat(config_get(CONFIG_WWW_ROOT), filepath);

    int fd = open(str_tocc(path), O_RDONLY | O_EXCL);
    if (fd < 0) {
        log_debug("open %r", path);
        rsp->status = 404;
        return rsp;
    }

    struct stat st;
    if (fstat(fd, &st) < 0) {
        log_debug("failed to stat '%r'", path);
        rsp->status = 404;
        return rsp;
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
