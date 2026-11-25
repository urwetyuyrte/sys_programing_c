#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <stdarg.h>

static FILE *logfile;
static int errors;
static void report(const char *fmt, ...) {
    va_list a; va_start(a, fmt); vprintf(fmt, a); va_end(a);
    va_start(a, fmt); vfprintf(logfile, fmt, a); va_end(a);
    fflush(logfile);
}
static int join(char *p, size_t size, const char *a, const char *b) {
    int n = snprintf(p, size, "%s/%s", a, b);
    if (n >= 0 && (size_t)n < size) return 1;
    report("ERROR: path too long: %s/%s\n", a, b);
    ++errors; return 0;
}
static int make_dir(const char *p) {
    if (!mkdir(p, 0755)) { report("Created: %s\n", p); return 1; }
    struct stat s;
    if (errno == EEXIST && !lstat(p, &s) && S_ISDIR(s.st_mode)) {
        report("Exists: %s\n", p); return 1;
    }
    report("ERROR: mkdir %s\n", p); ++errors; return 0;
}
static const char *category(const char *name) {
    const char *e = strrchr(name, '.');
    if (!e) return "Other";
    if (!strcasecmp(e,".txt") || !strcasecmp(e,".md")) return "Documents";
    if (!strcasecmp(e,".c") || !strcasecmp(e,".h") ||
        !strcasecmp(e,".go")) return "Sources";
    if (!strcasecmp(e,".jpg") || !strcasecmp(e,".png")) return "Images";
    if (!strcasecmp(e,".pdf")) return "PDFs";
    return "Other";
}
static void scan(const char *src, const char *dst) {
    DIR *d = opendir(src);
    if (!d) { report("ERROR: opendir %s\n", src); ++errors; return; }
    struct dirent *e;
    while ((e = readdir(d))) {
        if (!strcmp(e->d_name,".") || !strcmp(e->d_name,"..")) continue;
        char p[4096], folder[4096], target[4096]; struct stat s;
        if (!join(p,sizeof p,src,e->d_name)) continue;
        if (lstat(p,&s)) { report("ERROR: stat %s\n",p); ++errors; continue; }
        if (S_ISDIR(s.st_mode)) scan(p,dst);
        else if (S_ISREG(s.st_mode)) {
            if (!join(folder,sizeof folder,dst,category(e->d_name)) ||
                !join(target,sizeof target,folder,e->d_name)) continue;
            /* link refuses to overwrite existing files. */
            if (link(p,target)) {
                report("ERROR: %s: %s\n",p,strerror(errno)); ++errors;
            } else if (unlink(p)) {
                report("ERROR: unlink %s: %s\n",p,strerror(errno)); ++errors;
            } else report("Moved: %s -> %s\n",p,target);
        } else report("Skipped special file: %s\n",p);
    }
    closedir(d);
}
int main(int argc, char **argv) {
    const char *src = NULL, *dst = NULL;
    struct option opts[] = {{"source",required_argument,0,'s'},
        {"dest",required_argument,0,'d'},{0,0,0,0}};
    int c;
    while ((c=getopt_long(argc,argv,"",opts,NULL)) != -1) {
        if (c=='s') src=optarg; else if (c=='d') dst=optarg;
        else return EXIT_FAILURE;
    }
    if (!src || !dst || optind != argc) {
        fprintf(stderr,"Usage: file_sorter --source DIR --dest DIR\n");
        return EXIT_FAILURE;
    }
    logfile=fopen("sort_report.log","w");
    if (!logfile) { perror("sort_report.log"); return EXIT_FAILURE; }
    report("Sorting started\n");
    char *a=realpath(src,NULL), *b=NULL; struct stat s;
    if (!a || stat(a,&s) || !S_ISDIR(s.st_mode)) {
        report("ERROR: invalid source\n"); ++errors; goto end;
    }
    if (!make_dir(dst)) goto end;
    b=realpath(dst,NULL);
    if (!b || !strcmp(a,b) || !strcmp(a,"/") ||
        (!strncmp(a,b,strlen(a)) && b[strlen(a)]=='/')) {
        report("ERROR: destination must be outside source\n");
        ++errors; goto end;
    }
    const char *cats[]={"Documents","Sources","Images","PDFs","Other"};
    for (size_t i=0;i<sizeof cats/sizeof *cats;i++) {
        char p[4096];
        if (!join(p,sizeof p,b,cats[i]) || !make_dir(p)) goto end;
    }
    scan(a,b);
end:
    report("Sorting finished. Errors: %d\n",errors);
    free(a); free(b); fclose(logfile);
    return errors ? EXIT_FAILURE : EXIT_SUCCESS;
}
