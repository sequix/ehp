#include "log.h"
#include "file.h"
#include "psignal.h"
#include <assert.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

// where to write received signals, differs from processes
int signal_pipefd[2] = {-1, -1};

static void signal_handler(int sig)
{
    int save_errno = errno;
    int msg = sig;
    write(signal_pipefd[1], (char*)&msg, 1);
    errno = save_errno;
}

void signal_add(int sig, void (handler)(int))
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    //sa.sa_flags |= SA_RESTART;
    sigfillset(&sa.sa_mask);
    if (sigaction(sig, &sa, NULL) < 0) {
        log_debug("signal: sigaction");
        exit(1);
    }
}

void signal_init(void)
{
    int ret = socketpair(PF_UNIX, SOCK_STREAM, 0, signal_pipefd);
    if (ret < 0) {
        log_debug("signal: pipe init failed");
        exit(1);
    }
    file_set_nonblock(signal_pipefd[1]);

    signal_add(SIGCHLD, signal_handler);
    signal_add(SIGTERM, signal_handler);
    signal_add(SIGINT, signal_handler);
    signal_add(SIGALRM, signal_handler);
    signal_add(SIGPIPE, SIG_IGN);
}
