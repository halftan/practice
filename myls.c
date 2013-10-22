/**
 * Author: HalFtaN
 *
 **/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <getopt.h>
#include <string.h>
#include <sys/ioctl.h>

#define OPT_STRING "lavrR"

/***************
 *  EXIT CODE  *
 ***************/
#define FAILURE_VAL 2

#define LIST_OPT      0x00000001
#define MULTI_DIR_OPT 0x00000002
#define REVERSE_OPT   0x00000004
#define RECURSIVE_OPT 0x00000010
#define ALL_OPT       0x00000100
#define VERBOSE_OPT   0x00001000

static void do_ls(char *dir, int flags);
static void print_usage(char *argv[]);
static void ls_print(char **entries, size_t dir_count, int flags);
static int dname_cmp(const void *, const void *);
static int get_terminal_width();
static int max(int a, int b);

int main(int argc, char *argv[])
{
    int opt;
    int flags, i;
    char *cwd = NULL;

    flags = 0;
    cwd = getcwd(NULL, 500);

    while ((opt = getopt(argc, argv, OPT_STRING)) != -1) {
        switch (opt) {
        case 'v':
            flags |= VERBOSE_OPT;
            break;
        case 'l':
            flags |= LIST_OPT;
            break;
        case 'R':
            flags |= RECURSIVE_OPT;
            flags |= MULTI_DIR_OPT;
            break;
        case 'r':
            flags |= REVERSE_OPT;
            break;
        case 'a':
            flags |= ALL_OPT;
            break;
        default:
            print_usage(argv);
            exit(FAILURE_VAL);
        }
    }

    if (optind == argc) {
        if (cwd)
            do_ls(cwd, flags);
        else
            do_ls(".", flags);
    }
    for (i = optind; i < argc; ++i)
        do_ls(argv[i], flags);

    if (cwd)
        free(cwd);
    return 0;
}

void print_usage(char *argv[]) {
    fprintf(stderr, "Usage: %s [-l] [-v] [-r]\n",
            argv[0]);
}

void do_ls(char *dirname, int flags) {
    DIR *dir_ptr;
    struct dirent *direntp;
    char **entries;
    size_t i;
    size_t dir_count, str_allocated;

    dir_count = 0;
    str_allocated = 20;
    entries = (char**) malloc(sizeof(char*) * str_allocated);

    if ((dir_ptr = opendir(dirname)) == NULL) {
        fprintf(stderr, "myls: cannot open %s\n", dirname);
    } else {
        while ( (direntp = readdir(dir_ptr)) != NULL ) {
            if (flags & ALL_OPT && direntp->d_name[0] == '.')
                entries[dir_count++] = strndup(direntp->d_name, NAME_MAX);
            else if (direntp->d_name[0] != '.')
                entries[dir_count++] = strndup(direntp->d_name, NAME_MAX);
            if (str_allocated == dir_count) {
                str_allocated += 20;
                entries = (char**) realloc(entries,
                        sizeof(char*) * str_allocated);
            }
        }

        if (flags & VERBOSE_OPT) {
            printf("Verbose info\n");
            printf("dir count: %d, str allocated: %d\n", dir_count, str_allocated);
            for (i = 0; i < dir_count; ++i)
                printf("%s\n", entries[i]);
            printf("Verbose info end.\n\n");
        }
        qsort(entries, dir_count, sizeof(char*), dname_cmp);
        closedir(dir_ptr);
        if (flags & MULTI_DIR_OPT)
            printf("%s:\n", dirname);
        ls_print(entries, dir_count, flags);
        for (i = 0; i < str_allocated; ++i) {
            free(entries[i]);
        }
        free(entries);
    }
}

void ls_print(char **entries, size_t dir_count, int flags) {
    int i, max_col;
    int width = get_terminal_width();

    max_col = 0;
    for (i = 0; i < (int)dir_count; ++i)
        max_col = max(strlen(entries[i]), max_col);

    if (flags & REVERSE_OPT) {
        for (i = dir_count - 1; i >= 0; --i)
            printf("%s\n", entries[i]);
    } else
        for (i = 0; i < (int)dir_count; ++i)
            printf("%s\n", entries[i]);
}

int dname_cmp(const void *p1, const void *p2) {
    return strcmp(*(const char**)p1, *(const char**)p2);
}

int get_terminal_width(void) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}

int max(int a, int b) {
    return a > b ? a : b;
}
