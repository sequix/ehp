#pragma once

#include "str.h"

int tcp_new_server(const str_t *ip, int port, int backlog);
