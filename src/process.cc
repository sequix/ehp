#include "process.h"
#include "config.h"
#include "log.h"
#include "net.h"
#include "mem.h"
#include "poller.h"
#include "psignal.h"
#include "file.h"
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>

// the global processes pool
prspool_t the_prspool = NULL;

// the worker process data
process_t the_worker = NULL;

// return 1 if current process is the master
int prspool_init(int nprs)
{
    bool is_master = false;

    the_prspool = talloc(prspool_st);
    if (!the_prspool) {
        log_debug("prspool: talloc(prspool_st)");
        exit(1);
    }

    the_prspool->listenfd = tcp_new_listenfd(config_get(CONFIG_ADDRESS),
		config_get_int(CONFIG_PORT), config_get_int(CONFIG_BACKLOG));
    if (the_prspool->listenfd < 0) {
        log_debug("prspool: listenfd");
        exit(1);
    }

    the_prspool->nprs = nprs;
    the_prspool->prs = stalloc(nprs, process_t);
    if (!the_prspool->prs) {
        log_debug("prspool: stalooc(nprs, process_t)");
        exit(1);
    }

    process_t *prs = the_prspool->prs;

    for (int i = 0; i < nprs; ++i) {
        prs[i] = talloc(process_st);
        if (!prs[i]) {
            log_debug("prspool: talloc(prs[%d])", i);
            exit(1);
        }

        int ret = socketpair(PF_UNIX, SOCK_STREAM, 0, prs[i]->pipefd);
        assert(ret == 0);

        prs[i]->pid = fork();
        assert(prs[i]->pid >= 0);
        if (prs[i]->pid > 0) {
            is_master = true;
            close(prs[i]->pipefd[0]);
            log_debug("created worker %d", prs[i]->pid);
            continue;
        } else {
            is_master = false;
            the_worker = prs[i];
            close(prs[i]->pipefd[1]);
            break;
        }
    }
    if (is_master) {
        log_info("listening on %r:%r with %r workers",
            config_get(CONFIG_ADDRESS), config_get(CONFIG_PORT),
            config_get(CONFIG_WORKER));
    }
    return is_master;
}

static void master_handle_signals(int fd, bool *stop)
{
    int stat;
    pid_t pid;
    int len = 0;
    bool closed = false;
    char *signals = file_read_nonblock(fd, &len, &closed);

    if (!signals) {
        log_debug("master reads signals failed");
    }

    if (closed) {
        log_debug("master signals pipe closed unexpectedly");
    }

    for (int i = 0; i < len; ++i) {
        log_debug("master received signal %d", signals[i]);
        switch(signals[i]) {
            case SIGCHLD:
                while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
                    for (int i = 0; i < the_prspool->nprs; ++i) {
                        if (the_prspool->prs[i]->pid == pid) {
                            log_debug("master joining worker %d", pid);
                            the_prspool->prs[i]->pid = -1;
                            break;
                        }
                    }
                }
                if (pid < 0 && errno == ECHILD) {
                    errno = 0;
                }
                *stop = true;
                for (int i = 0; i < the_prspool->nprs; ++i) {
                    if (the_prspool->prs[i]->pid != -1) {
                        *stop = false;
                        break;
                    }
                }
                break;
            case SIGTERM:
            case SIGINT:
                for (int i = 0; i < the_prspool->nprs; ++i) {
                    if (the_prspool->prs[i]->pid != -1) {
                        log_debug("master sends SIGTERM to worker %d",
                                the_prspool->prs[i]->pid);
                        kill(the_prspool->prs[i]->pid, SIGTERM);
                    }
                }
                break;
            default:
                break;
        }
    }
}

void prspool_run_master(void)
{
    // round-robin index
    int rridx = 0;

    // whether to stop the server
    bool stop = false;

    poller_t plr = poller_new(5);
    int listenfd = the_prspool->listenfd;

    signal_init();
    poller_addfd(plr, signal_pipefd[0]);
    poller_addfd(plr, the_prspool->listenfd);

    while (!stop) {
        poller_event_t ev = poller_wait(plr);
        if (ev == NULL) {
            continue;
        }

        // new connection, inform one of the worker to handle it
        if (ev->fd == listenfd && ev->type == POLLER_EVENT_TYPE_READ) {
            char new_conn = 1;
            write(the_prspool->prs[rridx]->pipefd[1], &new_conn,
                    sizeof(new_conn));
            rridx = (rridx + 1) % the_prspool->nprs;
            continue;
        }

        // signal handle
        if (ev->fd == signal_pipefd[0] && ev->type == POLLER_EVENT_TYPE_READ) {
            master_handle_signals(ev->fd, &stop);
            continue;
        }
    }
    poller_close(plr);
}

static void worker_handle_signals(int fd, bool *stop)
{
    int stat;
    int len = 0;
    bool closed = false;
    char *signals = file_read_nonblock(fd, &len, &closed);

    if (!signals) {
        log_debug("worker reads signals failed");
        return;
    }

    if (closed) {
        log_debug("worker signals pipe closed unexpectedly");
        return;
    }

    for (int i = 0; i < len; ++i) {
        log_debug("worker received signal %d", signals[i]);
        switch(signals[i]) {
            case SIGCHLD:
                while (waitpid(-1, &stat, WNOHANG) > 0)
                    continue;
                break;
            case SIGTERM:
            case SIGINT:
                *stop = true;
                break;
            case SIGALRM:
                ptimer_signal_handler();
                break;
            default:
                break;
        }
    }
}

void prspool_run_worker(void)
{
    // whether to stop the server
    bool stop = false;

    // pipe between current worker and master
    int *worker_pipe = the_worker->pipefd;

    // where to receive new connection
    int listenfd = the_prspool->listenfd;

    // connection timeout in milliseconds
    uint64_t timeout_in_ms = config_get_int(CONFIG_TIMEOUT) * 1000;

    // TODO: nonblocking write
    // for nonblocking write, where the last write go
    //map_t wm_pos = map_new(4096);
    // for nonblocking write, what content to write
    //map_t wm_content = map_new(4096);

    ptimer_init();
    signal_init();
    poller_t plr = poller_new(5);
    the_worker->plr = plr;

    poller_addfd(plr, signal_pipefd[0]);
    poller_addfd(plr, worker_pipe[0]);

    while (!stop) {
        poller_event_t ev = poller_wait(plr);

        if (ev == NULL) {
            continue;
        }

        // new connection
        if (ev->fd == worker_pipe[0] && ev->type == POLLER_EVENT_TYPE_READ) {
            int connfd = tcp_accept(listenfd);
            if (connfd < 0) continue;
            poller_addfd(plr, connfd);
            ptimer_add(ptime_epoch(), timeout_in_ms, connfd);
            continue;
        }

        // signal handle
        if (ev->fd == signal_pipefd[0] && ev->type == POLLER_EVENT_TYPE_READ) {
            worker_handle_signals(ev->fd, &stop);
            continue;
        }

        // IO
        if (ev->type == POLLER_EVENT_TYPE_READ) {
            ptimer_update(ptime_epoch(), ev->fd);

            int len = 0;
            bool closed = false;
            char *raw_body = file_read_nonblock(ev->fd, &len, &closed);

            if (closed) {
                log_debug("connection %r closed", socket_peer_addr(ev->fd));
                poller_delfd(plr, ev->fd);
                continue;
            }

            http_req_t req = http_req_new(raw_body, len, ev->fd);
            log_info("%r %s %s", req->remote_addr, req->method, req->addr->path);

            http_rsp_t rsp = http_rsp_from_file(req->addr->path);
            http_rsp_write(rsp, ev->fd);
            continue;
        }

        // TODO: IO nonblocking Write
        //if (ev->type == POLLER_EVENT_TYPE_WRITE) {
        //continue;
        //}
    }
    close(signal_pipefd[1]);
    poller_close(plr);
    log_debug("worker terminated");
}
