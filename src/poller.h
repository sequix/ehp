#pragma once

#include "http.h"
#include "types.h"
#include <stdint.h>
#include <sys/epoll.h>

typedef uint8_t poller_event_type_t;
#define POLLER_EVENT_TYPE_READ  ((poller_event_type_t)0x0)
#define POLLER_EVENT_TYPE_WRITE ((poller_event_type_t)0x1)
#define POLLER_EVENT_TYPE_ERROR ((poller_event_type_t)0x2)

typedef struct {
    int fd;
    poller_event_type_t type;
} poller_event_st;

typedef poller_event_st* poller_event_t;

typedef struct {
    // file descriptor that epoll needs
    int epollfd;

    // max number of events of once epoll can return
    len_t cap;

    /// number of events epoll returned
    len_t size;

    // events epoll returned
    struct epoll_event *events;

    // fds that are being listened
    map_t fds;
} poller_st;

typedef poller_st* poller_t;

poller_t poller_new(len_t cap);

void poller_addfd(poller_t plr, int fd);

void poller_delfd(poller_t plr, int fd);

poller_event_t poller_wait(poller_t plr);

void poller_close(poller_t plr);
