#pragma once

// where to write received signals, differs from processes
extern int signal_pipefd[2];

void signal_init(void);

void signal_add(int sig, void (handler)(int));
