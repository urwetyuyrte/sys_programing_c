#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include <string.h>
int main(int argc, char **argv) {
    int physical = 0, c;
    static const struct option opts[] = {
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {"author", no_argument, 0, 'a'}, {0,0,0,0}
    };
    while ((c = getopt_long(argc, argv, "LP", opts, NULL)) != -1) {
        switch (c) {
        case 'L': physical = 0; break;
        case 'P': physical = 1; break;
        case 'h': puts("Usage: pwd_80_22 [-L|-P] [OPTIONS]\n-L logical path (default)\n-P physical path\n--help help\n--version version\n--author developer"); return 0;
        case 'v': puts("pwd_80_22 1.0.0"); return 0;
        case 'a': puts("Розробник: Семисал Гліб"); return 0;
        default: return 1;
        }
    }
    if (optind != argc) { fputs("Unexpected operand\n", stderr); return 1; }
    const char *pwd = getenv("PWD");
    struct stat a,b;
    if (!physical && pwd && pwd[0]=='/' &&
        stat(pwd,&a)==0 && stat(".",&b)==0 &&
        a.st_dev==b.st_dev && a.st_ino==b.st_ino) {
        puts(pwd); return 0;
    }
    char *path = getcwd(NULL,0);
    if (!path) { perror("getcwd"); return 1; }
    puts(path); free(path); return 0;
}
