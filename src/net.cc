#include "net.h"
#include "util.h"
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h> // inet_pton

int tcp_new_listenfd(str_t ip, int port, int backlog)
{
    struct sockaddr_in sa = {0};
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    sa.sin_family = AF_INET;
    inet_pton(AF_INET, str_tocc(ip), &(sa.sin_addr));
    sa.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
        return -1;
    }
    file_set_nonblock(sockfd);
    listen(sockfd, backlog);
    return sockfd;
}
