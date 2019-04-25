#include "poller.h"
#include "log.h"
#include "util.h"
#include <stdio.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

poller_t poller_new(int listenfd, len_t cap, int timeout)
{
    int epollfd = epoll_create(cap);
    if (epollfd < 0) {
        return NULL;
    }

    struct epoll_event ev = {0};
    ev.events = EPOLLIN;
    ev.data.fd = listenfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev) < 0) {
        close(epollfd);
        return NULL;
    }

    poller_t plr = talloc(poller_st);
    if (plr == NULL) {
        close(epollfd);
        return NULL;
    }

    plr->epollfd  = epollfd;
    plr->listenfd = listenfd;
    plr->timeout  = timeout;
    plr->cap      = cap;
    plr->size     = 0;
    plr->events   = talloc(struct epoll_event);
    if (plr->events == NULL) {
        close(epollfd);
        free(plr);
        return NULL;
    }
    return plr;
}

static void poller_handle_new_conn(poller_t *plr)
{
    struct epoll_event ev = {0};

    while (1) {
        int newfd = accept(plr->listenfd, NULL, NULL);
        if (newfd < 0) {
            if (errno == EWOULDBLOCK) {
                break;
            }
            log_error("accept()");
            continue;
        }
        log_debug("new conn at %d", newfd);
        file_set_nonblock(newfd);

        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = newfd;
        if (epoll_ctl(plr->epollfd, EPOLL_CTL_ADD, newfd, &ev) < 0) {
            log_error("epoll_ctl(EPOLL_CTL_ADD)");
        }
    }
}

static http_req_t *poller_handle_request(poller_t *plr, int fd)
{
    bool close_conn = false;
    ssize_t total_len = 0;
    ssize_t size = BUFSIZ;
    char *buf = (char*)malloc(size);
    char *pbuf = buf;
    struct epoll_event ev = {0};

    while (1) {
        log_debug("new request from %d", fd);
        int len = recv(fd, pbuf, size, 0);
        if (len < 0) {
            if (errno == EWOULDBLOCK) {
                errno = 0;
                break;
            }
            log_error("recv()");
            continue;
        }

        if (len == 0) {
            close_conn = true;
            break;
        }
        total_len += len;
        pbuf = buf + total_len;

        if (len < size) {
            break; 
        }

        if (len == size) {
            size = size << 1;
            buf = (char*)realloc(buf, size);
        }
    }
    if (close_conn) {
        log_debug("closing conn %d", fd);
        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = fd;
        epoll_ctl(plr->epollfd, EPOLL_CTL_DEL, fd, &ev);
        close(fd);
        return NULL;
    }
    return http_req_new(buf, total_len, fd);
}

http_req_t *poller_wait(poller_t *plr)
{
    while (1) {
        if (plr->size == 0) {
            log_debug("waiting inside...");
            int nfds = epoll_wait(plr->epollfd, plr->events, plr->cap,
                    plr->timeout);
            log_debug("nfds: %d", nfds);
            if (nfds < 0) {
                return NULL;
            }
            plr->size = nfds;
        }

        while (plr->size > 0) {
            int fd = plr->events[--plr->size].data.fd;
            if (fd == plr->listenfd) {
                poller_handle_new_conn(plr);
                continue;
            }
            http_req_t *ret = poller_handle_request(plr, fd);
            if (ret != NULL) {
                return ret;
            }
        }
    }
}
