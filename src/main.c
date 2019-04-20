#include "stdio.h"
#include "poller.h"
#include "net.h"
#include "log.h"
#include "args.h"
#include "regexp.h"
#include "config.h"
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    regexp_init();
    args_handle(argc, argv);
    config_init(*args_config_file->filename);
    log_init(config_get("log_file"));

    int listenfd = tcp_new_server(config_get("address"),
        config_get_int("port"), config_get_int("backlog"));

    poller_t *plr = poller_new(listenfd, 10, -1);
    while (1) {
        http_req_t *req = poller_wait(plr);

        // TODO: routing & response wraping & req releasing
        char path[8192];
        strcpy(path, config_get("www_root"));
        strcpy(path+strlen(path), req->addr->path);
        int len = strlen(path);
        if (path[len-1] == '/') {
            strcpy(path + len, "index.html");
        }
        int fd = open(path, O_RDONLY, 0);
        if (fd < 0) {
            // TODO: 404
            continue;
        }
        write(req->clientfd, "HTTP/1.1 200 OK\r\n", 17);
        write(req->clientfd, "Content-Length: 11\r\n\r\n", 22);
        write(req->clientfd, "hello world", 11);
        log_debug("wrote response");
    }
}
