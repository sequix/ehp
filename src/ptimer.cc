#include "log.h"
#include "net.h"
#include "psignal.h"
#include "process.h"
#include "ptimer.h"
#include "mem.h"
#include "map.h"
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <stdlib.h>

#define PTIMER_INIT_SIZE 8

// node relation
#define parent(x) (((x)-1) >> 1)
#define left(x)   (((x)<<1) + 1)
#define right(x)  (((x)<<1) + 2)

// the the_timer for current process
ptimer_t the_timer = NULL;

static void ptimer_disable_sigalrm(void)
{
    sigset_t set;
    sigprocmask(SIG_SETMASK, NULL, &set);
    sigaddset(&set, SIGALRM);
    sigprocmask(SIG_SETMASK, &set, NULL);
}

static void ptimer_enable_sigalrm(void)
{
    sigset_t set;
    sigprocmask(SIG_SETMASK, NULL, &set);
    sigdelset(&set, SIGALRM);
    sigprocmask(SIG_SETMASK, &set, NULL);
}

void ptimer_init(void)
{
    the_timer = talloc(ptimer_st);
    the_timer->heap = stalloc(PTIMER_INIT_SIZE, ptimer_node_t);
    the_timer->len = 0;
    the_timer->cap = PTIMER_INIT_SIZE;
    the_timer->updated_at = map_new();
}

static void ptimer_heap_realloc(void)
{
    len_t new_cap = the_timer->cap << 1;
    the_timer->heap = stralloc(the_timer->heap, new_cap, ptimer_node_t);
    if (!the_timer->heap) {
        log_error("timer heap realloc failed");
        exit(1);
    }
    the_timer->cap = new_cap;
}

static void ptimer_heap_push(ptimer_node_t node)
{
    ptimer_disable_sigalrm();
    if (the_timer->len >= the_timer->cap) {
        ptimer_heap_realloc();
    }
    int p = the_timer->len;
    ptimer_node_t *heap = the_timer->heap;

    for (int i = parent(p); i >= 0; i = parent(i)) {
        if (heap[i]->due_epoch < node->due_epoch) {
            break;
        }
        heap[p] = heap[i];
        p = i;
    }
    heap[p] = node;
    ++the_timer->len;
    ptimer_enable_sigalrm();
}

static ptimer_node_t ptimer_heap_pop(void)
{
    ptimer_disable_sigalrm();
    int len = the_timer->len;
    ptimer_node_t *heap = the_timer->heap;

    ptimer_node_t n = heap[0];
    ptimer_node_t nt = heap[len-1];

    int p = 0;
    for (int i = left(p); i < len; i = left(i)) {
        if (i+1 < len && heap[i+1]->due_epoch < heap[i]->due_epoch) {
            ++i;
        }
        if (nt->due_epoch < heap[i]->due_epoch) {
            break;
        }
        heap[p] = heap[i];
        p = i;
    }
    heap[p] = nt;
    --the_timer->len;
    ptimer_enable_sigalrm();
    return n;
}

void ptimer_run(void)
{
    if (the_timer->len <= 0) return;
    uint64_t now = ptime_epoch();
    ptimer_node_t node = the_timer->heap[0];

    uint64_t delay = node->dealy_in_millisec;

    uint64_t updated_at =
        map_get_uint64(the_timer->updated_at, str_from_uint64(node->fd));

    uint64_t delaied = now - updated_at;

    struct itimerval timeout;
    memset(&timeout, 0, sizeof(timeout));
    if (delaied < delay) {
        uint64_t remaining_delay = delay - delaied;
        timeout.it_value.tv_sec = remaining_delay / 1000;
        timeout.it_value.tv_usec = remaining_delay % 1000 * 1000;
    }
    setitimer(ITIMER_REAL, &timeout, NULL);
}

void ptimer_add(uint64_t now, uint64_t delay_in_millisec, int fd)
{
    ptimer_node_t node = talloc(ptimer_node_st);
    assert(node);

    node->fd = fd;
    node->due_epoch = now + delay_in_millisec;
    node->dealy_in_millisec = delay_in_millisec;
    ptimer_heap_push(node);
    ptimer_update(now, fd);
    ptimer_run();
}

void ptimer_update(uint64_t now, int fd)
{
    ptimer_disable_sigalrm();
    map_set(the_timer->updated_at, str_from_uint64(fd), str_from_uint64(now));
    ptimer_enable_sigalrm();
}

void ptimer_signal_handler(void)
{
    ptimer_node_t node = the_timer->heap[0];
    uint64_t updated_epoch = map_get_uint64(the_timer->updated_at,
            str_from_uint64(node->fd));
    uint64_t now_epoch = ptime_epoch();

    uint64_t real_dealy = now_epoch - updated_epoch;
    if (real_dealy >= node->dealy_in_millisec) {
        log_debug("connection %r timeout", socket_peer_addr(node->fd));
        poller_delfd(the_worker->plr, node->fd);
    } else {
        ptimer_add(updated_epoch, node->dealy_in_millisec, node->fd);
    }
    ptimer_heap_pop();
    ptimer_run();
}
