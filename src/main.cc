#include "file.h"
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

// TODO: reconstruct the bottom data structures
// TODO: deal memory in a more elegent & consistent way

int main(int argc, char *argv[])
{
    regexp_init();
    args_handle(argc, argv);
    config_init(*args_config_file->filename);
    log_init(config_get(CONFIG_LOG_FILE));

    int listenfd = tcp_new_server(config_get(CONFIG_ADDRESS),
        config_get_int(CONFIG_PORT), config_get_int(CONFIG_BACKLOG));

    // TODO: only worker process the requests & health check from master
    int nworker = config_get_int(CONFIG_WORKER);
    for (int i = 0; i < nworker; ++i) {
        int ret = fork();
        if (ret < 0) {
            log_error("fork()");
            exit(1);
        }
        if (ret == 0) {
            break;
        }
        log_info("created worker %d", ret);
    }

    // TODO: register signal handler before starting wait children

    poller_t *plr = poller_new(listenfd, 10, -1);

    char buf[128];
    char path[8192];
    str_t *www_root = config_get(CONFIG_WWW_ROOT);
    int www_root_len = www_root->len;
    strcpy(path, www_root->s);

    while (1) {
        http_req_t *req = poller_wait(plr);
        if (req == NULL) {
            continue;
        }
        // TODO: address also should be printed
        log_info("%s %s %s", req->method, req->addr->path, req->version);

        // TODO: routing & response wraping & req releasing
        strcpy(path + www_root_len, req->addr->path);
        int len = strlen(path);
        if (path[len-1] == '/') {
            strcpy(path + len, "index.html");
        }

        http_rsp_t *rsp = http_rsp_new();
        rsp->version = HTTP_VERSION_1_1;

        char *rsp_body = file_read(path, &rsp->len);
        if (rsp_body == NULL) {
            log_debug("file_read wrong");
            rsp->status = 404;
            http_rsp_write(rsp, req->clientfd);
            log_debug("wrote 404 response");
            continue;
        }

        rsp->status = 200;
        rsp->body = rsp_body;
        http_rsp_write(rsp, req->clientfd);
        log_debug("wrote 200 response");
    }
}
