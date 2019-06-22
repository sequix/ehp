#include "log.h"
#include "net.h"
#include "file.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h> // inet_pton

int tcp_new_listenfd(str_t ip, int port, int backlog)
{
    int reuse = 1;
    struct sockaddr_in sa = {0};
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        return -1;
    }

    sa.sin_family = AF_INET;
    inet_pton(AF_INET, str_tocc(ip), &(sa.sin_addr));
    sa.sin_port = htons(port);

    if (bind(fd, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
        return -1;
    }
    file_set_nonblock(fd);
    listen(fd, backlog);
    return fd;
}

str_t socket_addr_to_str(const struct sockaddr_in *addr)
{
    char buf[128];
    inet_ntop(AF_INET, &addr->sin_addr, buf, sizeof(buf));
    sprintf(buf + strlen(buf), ":%d", ntohs(addr->sin_port));
    return str_from(buf);
}

int tcp_accept(int listenfd)
{
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    int connfd = accept(listenfd, (struct sockaddr*)&addr, &addrlen);
    if (connfd < 0) {
        log_debug("failed to accept connection");
        return connfd;
    }
    log_debug("new connection from %r", socket_addr_to_str(&addr));
    return connfd;
}

str_t socket_peer_addr(int sockfd)
{
    struct sockaddr_in addr;
    len_t addrlen = sizeof(addr);
    getpeername(sockfd, (struct sockaddr *)&addr, &addrlen);
    return socket_addr_to_str(&addr);
}

str_t socket_addr(int sockfd)
{
    struct sockaddr_in addr;
    len_t addrlen = sizeof(addr);
    getsockname(sockfd, (struct sockaddr *)&addr, &addrlen);
    return socket_addr_to_str(&addr);
}
