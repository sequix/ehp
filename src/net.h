#pragma once

#include "str.h"
#include <netinet/in.h>

int tcp_new_listenfd(str_t ip, int port, int backlog);

int tcp_accept(int listenfd);

// return peer address as str_t
str_t socket_peer_addr(int sockfd);

// return socket binding address as str_t
str_t socket_addr(int sockfd);

// convert sockaddr_in to str_t
str_t socket_addr_to_str(const struct sockaddr_in *addr);
