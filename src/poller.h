#pragma once

#include "http.h"
#include <sys/epoll.h>

typedef struct {
    // file descriptor that epoll needs
    int epollfd;

    // file descriptor the server listen to
    int listenfd;

    // wait timeout in milliseconds
    int timeout;

    // max number of events of once epoll can return
    size_t cap;

    /// number of events epoll returned
    size_t size;

    // events epoll returned
    struct epoll_event *events;
} poller_t;

poller_t *poller_new(int listenfd, size_t cap, int timeout);

http_req_t *poller_wait(poller_t *plr);
