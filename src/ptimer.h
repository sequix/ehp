#pragma once

#include "types.h"
#include "lock.h"
#include "ptime.h"
#include "map.h"
#include <stdint.h>

typedef struct {
    int fd;
    uint64_t due_epoch;
    uint64_t dealy_in_millisec;
} ptimer_node_st;

typedef ptimer_node_st* ptimer_node_t;

typedef struct {
    len_t len;
    len_t cap;
    map_t updated_at;
    ptimer_node_t *heap;
} ptimer_st;

typedef ptimer_st* ptimer_t;

// the timer for current process
extern ptimer_t the_timer;

void ptimer_init(void);

void ptimer_add(uint64_t now, uint64_t delay_in_millisec, int fd);

void ptimer_update(uint64_t now, int fd);

void ptimer_signal_handler(void);
