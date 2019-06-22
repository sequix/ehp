#include "map.h"
#include "mem.h"
#include "file.h"
#include "log.h"
#include "poller.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>

poller_t poller_new(len_t cap)
{
    int epollfd = epoll_create(cap);
    if (epollfd < 0) {
        log_error("poller: epoll_create");
        exit(1);
    }

    poller_t plr = talloc(poller_st);
    if (!plr) {
        log_error("poller: init poller_t failed");
        exit(1);
    }

    plr->epollfd = epollfd;
    plr->cap = cap;
    plr->size = 0;

    plr->events = stalloc(cap, struct epoll_event);
    if (!plr->events) {
        log_error("poller: init poller_t.events failed");
        exit(1);
    }

    plr->fds = map_new(4096);
    if (!plr->fds) {
        log_error("poller: init plr->fds");
        exit(1);
    }
    return plr;
}

void poller_addfd(poller_t plr, int fd)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(plr->epollfd, EPOLL_CTL_ADD, fd, &event);

    file_set_nonblock(fd);
    map_set(plr->fds, str_from_uint64(fd), str_from("1"));
}

void poller_delfd(poller_t plr, int fd)
{
    epoll_ctl(plr->epollfd, EPOLL_CTL_DEL, fd, 0);
    close(fd);
    map_set(plr->fds, str_from_uint64(fd), str_from("0"));
}

poller_event_t poller_wait(poller_t plr)
{
    while (1) {
        if (plr->size == 0) {
            int nfds = epoll_wait(plr->epollfd, plr->events, plr->cap, -1);
            if (nfds < 0) {
                log_debug("poller: epoll_wait failed");
                return NULL;
            }
            plr->size = nfds;
        }

        while (plr->size > 0) {
            struct epoll_event epev = plr->events[--plr->size];
            poller_event_t ev = talloc(poller_event_st);
            ev->fd = epev.data.fd;
            switch (epev.events) {
                case EPOLLIN:
                    ev->type = POLLER_EVENT_TYPE_READ;
                    break;
                case EPOLLOUT:
                    ev->type = POLLER_EVENT_TYPE_WRITE;
                    break;
                case EPOLLERR:
                    ev->type = POLLER_EVENT_TYPE_ERROR;
                    break;
                default:
                    break;
            }
            return ev;
        }
    }
}

static int poller_close_help(str_t key, str_t value)
{
    if (str_eq(value, "1")) {
        int fd = atoi(str_tocc(key));
        close(fd);
    }
    return 0;
}

void poller_close(poller_t plr)
{
    map_iter(plr->fds, poller_close_help);
    close(plr->epollfd);
}
