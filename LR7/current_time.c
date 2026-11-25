#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
int main(int argc, char **argv) {
    char path[4096], stamp[64];
    const char *home = getenv("HOME");
    if (argc > 2) return 2;
    if (argc == 2) {
        if (snprintf(path, sizeof path, "%s", argv[1]) >= (int)sizeof path) return 1;
    } else {
        if (!home || snprintf(path, sizeof path, "%s/current_time_80_22.log", home) >= (int)sizeof path) return 1;
    }
    time_t now = time(NULL);
    struct tm local;
    if (now == (time_t)-1 || !localtime_r(&now, &local) ||
        !strftime(stamp, sizeof stamp, "%Y-%m-%d %H:%M:%S %z", &local)) return 1;
    FILE *log = fopen(path, "a");
    if (!log) { perror(path); return 1; }
    int ok = fprintf(log, "%s\n", stamp) >= 0;
    if (fclose(log) != 0) ok = 0;
    return ok ? 0 : 1;
}
