#pragma once

#include "str.h"

int tcp_new_listenfd(str_t ip, int port, int backlog);
