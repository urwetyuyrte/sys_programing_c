#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
static volatile sig_atomic_t reload = 0;
static char logLevel[32] = "info";
static int logInterval = 5;
static void handle_usr1(int sig) { (void)sig; reload = 1; }
static int loadConfig(void) {
    FILE *f = fopen("config.txt", "r");
    char level[32]; int interval;
    if (!f) { perror("config.txt"); return 0; }
    int ok = fscanf(f, "%31s %d", level, &interval);
    fclose(f);
    if (ok != 2 || interval < 1 || interval > 86400) {
        fputs("Invalid configuration\n", stderr); return 0;
    }
    strcpy(logLevel, level); logInterval = interval;
    printf("[CONFIG LOADED] logLevel: %s, logInterval: %d\n", logLevel, logInterval);
    return 1;
}
int main(void) {
    struct sigaction sa = {0};
    sa.sa_handler = handle_usr1; sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) == -1) return 1;
    if (!loadConfig()) return 1;
    printf("PID: %ld\nkill -USR1 %ld\n", (long)getpid(), (long)getpid());
    for (;;) {
        if (reload) { reload = 0; loadConfig(); }
        printf("[%s] System message (interval %d sec)\n", logLevel, logInterval);
        fflush(stdout);
        for (int i = 0; i < logInterval && !reload; ++i) sleep(1);
    }
}
