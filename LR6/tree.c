#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

static int dirs, files;
static void make_dir(const char *p) {
    if (mkdir(p, 0755) && errno != EEXIST) {
        perror(p); exit(EXIT_FAILURE);
    }
}
static void make_file(const char *p) {
    FILE *f = fopen(p, "w");
    if (!f) { perror(p); exit(EXIT_FAILURE); }
    if (fclose(f)) { perror(p); exit(EXIT_FAILURE); }
}
static void tree(const char *path, int depth) {
    DIR *d = opendir(path);
    if (!d) { perror(path); exit(EXIT_FAILURE); }
    struct dirent *e;
    while ((e = readdir(d))) {
        if (e->d_name[0] == '.') continue;
        char p[4096]; struct stat s, link;
        int n = snprintf(p, sizeof p, "%s/%s", path, e->d_name);
        if (n < 0 || (size_t)n >= sizeof p) exit(EXIT_FAILURE);
        if (lstat(p, &link) || stat(p, &s)) {
            perror(p); exit(EXIT_FAILURE);
        }
        printf("%*s|-- %s\n", depth * 4, "", e->d_name);
        if (S_ISDIR(s.st_mode) && !S_ISLNK(link.st_mode)) {
            ++dirs; tree(p, depth + 1);
        } else ++files;
    }
    closedir(d);
}
int main(int argc, char **argv) {
    const char *root = "lab6_22";
    if (argc == 1) {
        make_dir(root);
        make_dir("lab6_22/.git");
        make_dir("lab6_22/src");
        make_dir("lab6_22/bin");
        make_dir("lab6_22/docs");
        make_dir("lab6_22/docs/img");
        make_file("lab6_22/.git/config");
        make_file("lab6_22/src/main.c");
        make_file("lab6_22/docs/manual.txt");
        make_file("lab6_22/docs/presentation.pdf");
        make_file("lab6_22/docs/img/cat.jpg");
        make_file("lab6_22/docs/img/dog.png");
        make_file("lab6_22/README.md");
    } else if (argc == 2) root = argv[1];
    else return EXIT_FAILURE;
    puts(root); tree(root, 0);
    printf("%d directories, %d files\n", dirs, files);
    return 0;
}
