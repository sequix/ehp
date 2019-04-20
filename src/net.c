#include "net.h"
#include "util.h"
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h> // inet_pton

int tcp_new_server(const char *ip, int port, int backlog)
{
    struct sockaddr_in sa = {0};
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    sa.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &(sa.sin_addr));
    sa.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
        return -1;
    }
    fd_set_nonblocking(sockfd);
    listen(sockfd, backlog);
    return sockfd;
}
