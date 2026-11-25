#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <execinfo.h>
static volatile sig_atomic_t requested = 0;
static void handle_quit(int sig) { (void)sig; requested = 1; }
int main(void) {
    struct sigaction sa = {0};
    sigset_t blocked, old;
    sigemptyset(&blocked); sigaddset(&blocked, SIGQUIT);
    if (sigprocmask(SIG_BLOCK, &blocked, &old) == -1) return 1;
    sa.sa_handler = handle_quit; sigemptyset(&sa.sa_mask);
    if (sigaction(SIGQUIT, &sa, NULL) == -1) return 1;
    printf("PID: %ld\nkill -3 %ld\nWaiting for SIGQUIT (Ctrl+\\)\n", (long)getpid(), (long)getpid());
    fflush(stdout);
    while (!requested) sigsuspend(&old);
    puts("--- Stack Trace (SIGQUIT) ---"); fflush(stdout);
    void *frames[32];
    int n = backtrace(frames, 32);
    backtrace_symbols_fd(frames, n, STDOUT_FILENO);
    return 0;
}
