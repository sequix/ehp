#pragma once

#include "ptimer.h"
#include "poller.h"

typedef struct {
    int pid;
    poller_t plr;
    int pipefd[2];
} process_st;

typedef process_st* process_t;

typedef struct {
    // number of processes
    int nprs;

    // the server listens at
    int listenfd;

    // all child processes
    process_t *prs;
} prspool_st;

typedef prspool_st* prspool_t;

// the global processes pool
extern prspool_t the_prspool;

// the worker process data
extern process_t the_worker;

int prspool_init(int nprs);

void prspool_run_master(void);

void prspool_run_worker(void);
