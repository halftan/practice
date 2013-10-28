/**
 * Author: HalFtaN
 *
 **/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <dirent.h>
#include <getopt.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>

#ifndef NAME_MAX
#define NAME_MAX 256
#endif

#define OPT_STRING "lavrcR"

/***************
 *  EXIT CODE  *
 ***************/
#define FAILURE_VAL 2

#define LIST_OPT      0x00000001
#define MULTI_DIR_OPT 0x00000002
#define REVERSE_OPT   0x00000004
#define IGCASE_OPT    0x00000008
#define RECURSIVE_OPT 0x00000010
#define ALL_OPT       0x00000100
#define VERBOSE_OPT   0x00001000

#define MAX(x, y)     x > y ? x : y

#define COLMAJ_IND(b, c_len, c, r) \
    b[c+r*c_len]

typedef struct entry_stat {
    char        * f_name;
    struct   stat f_stat;
    unsigned  int name_len;
} entry_stat;

typedef struct col_info {
    size_t   col_count;
    size_t * col_len;
} col_info;

char *my_name;
char *cwd = NULL;
unsigned int flags;

static     void do_ls(char *dir);
static     void print_usage(char *argv[]);
static     void ls_print(entry_stat **entries, size_t dir_count);
static      int get_terminal_width();
static     void copy_entry(entry_stat **entries, struct dirent *direntp, size_t dir_count);
static     void free_entry(entry_stat **entries, size_t count);
static col_info calc_col_info(entry_stat **entries, size_t count);
static     void clear_col_info(col_info base);

// used for qsort
static      int dname_cmp(const void *, const void *);
static      int namlen_cmp(const void *p1, const void *p2);

int main(int argc, char *argv[])
{
    int opt;
    int i;

    flags = 0;
    cwd = getcwd(NULL, 500);
    my_name = argv[0];

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
        case 'c':
            flags |= IGCASE_OPT;
            break;
        default:
            print_usage(argv);
            exit(FAILURE_VAL);
        }
    }

    if (optind == argc) {
        if (cwd)
            do_ls(cwd);
        else
            do_ls(".");
    } else {
        for (i = optind; i < argc; ++i)
            do_ls(argv[i]);
    }
    if (cwd)
        free(cwd);
    return 0;
}

void print_usage(char *argv[]) {
    fprintf(stderr, "Usage: %s [-l] [-v] [-r]\n",
            argv[0]);
}

void do_ls(char *dirname) {
    DIR *dir_ptr;
    struct dirent *direntp;
    entry_stat **entries;
    size_t i;
    size_t dir_count, st_allocated;

    // chdir to destination directory
    chdir(dirname);

    dir_count = 0;
    st_allocated = 20;
    entries = (entry_stat**) malloc(sizeof(entry_stat*) * st_allocated);

    if ((dir_ptr = opendir(dirname)) == NULL) {
        fprintf(stderr, "%s: cannot open %s\n", my_name, dirname);
    } else {
        while ( (direntp = readdir(dir_ptr)) != NULL ) {
            if (flags & ALL_OPT && direntp->d_name[0] == '.')
                copy_entry(entries, direntp, dir_count++);
            else if (direntp->d_name[0] != '.')
                copy_entry(entries, direntp, dir_count++);
            if (st_allocated == dir_count) {
                st_allocated += 20;
                entries = (entry_stat**) realloc(entries,
                        sizeof(entry_stat*) * st_allocated);
            }
        }

        if (flags & VERBOSE_OPT) {
            printf("Verbose info\n");
            printf("dir count: %zu, entries allocated: %zu\n", dir_count, st_allocated);
            for (i = 0; i < dir_count; ++i)
                printf("%s\n", entries[i]->f_name);
            printf("Verbose info end.\n\n");
        }
        qsort(entries, dir_count, sizeof(char*), dname_cmp);
        closedir(dir_ptr);

        if (flags & MULTI_DIR_OPT)
            printf("%s:\n", dirname);
        ls_print(entries, dir_count);

        free_entry(entries, st_allocated);
    }

    // chdir back to original directory.
    if (cwd != NULL)
        chdir(cwd);
}

void ls_print(entry_stat **entries, size_t dir_count) {
    int i;
    col_info colinfo;
    char **pstr;

    if (! (flags & LIST_OPT)) {
        colinfo = calc_col_info(entries, dir_count);
        pstr = (char**) malloc(sizeof(char*) * colinfo.col_count);
        for (i = 0; i < (int)colinfo.col_count; ++i) {
            pstr[i] = (char*) malloc(sizeof(char*) * NAME_MAX);
            sprintf(pstr[i], "%%%zus", colinfo.col_len[i]);
        }

        if (flags & VERBOSE_OPT)
            for (i = 0; i < (int)colinfo.col_count; ++i)
                printf("%s\n", pstr[i]);

        if (flags & REVERSE_OPT) {
            for (i = dir_count - 1; i >= 0; --i)
                printf(pstr[i % colinfo.col_count], entries[i]->f_name);
        } else {
            for (i = 0; i < (int)dir_count; ++i)
                printf(pstr[i % colinfo.col_count], entries[i]->f_name);
        }

        clear_col_info(colinfo);
    }

    if (flags & REVERSE_OPT) {
        for (i = dir_count - 1; i >= 0; --i)
            printf("%s\n", entries[i]->f_name);
    } else
        for (i = 0; i < (int)dir_count; ++i)
            printf("%s\n", entries[i]->f_name);
}

void copy_entry(entry_stat **entries, struct dirent *direntp, size_t dir_count) {
    // allocate and clear memory
    entries[dir_count] = (entry_stat*) malloc(sizeof(entry_stat));
    memset(entries[dir_count], 0, sizeof(entry_stat));
    // set data in struct
    entries[dir_count]->f_name = strndup(direntp->d_name, NAME_MAX);
    entries[dir_count]->name_len = direntp->d_namlen;
    stat(direntp->d_name, &entries[dir_count]->f_stat);
}

void free_entry(entry_stat **entries, size_t count) {
    size_t i;
    for (i = 0; i < count; ++i) {
        free(entries[i]->f_name);
        free(entries[i]);
    }
    free(entries);
}

int dname_cmp(const void *p1, const void *p2) {
    if (flags & IGCASE_OPT)
        return strcasecmp(*(const char**)p1, *(const char**)p2);
    else
        return strcmp(*(const char**)p1, *(const char**)p2);
}

int namlen_cmp(const void *p1, const void *p2) {
    return *(unsigned int *)p2 - *(unsigned int *)p1;
}

col_info calc_col_info(entry_stat **entries, size_t count) {
    int i, j, width, cal_width, cal_col_count;
    col_info info;
    unsigned int *len_info;

    len_info = (unsigned int*) malloc(sizeof(unsigned int) * count);
    cal_col_count = 0;
    cal_width = 0;
    width = get_terminal_width();

    for (i = 0; i < (int)count; ++i)
        len_info[i] = entries[i]->name_len;

    qsort(len_info, count, sizeof(unsigned int), namlen_cmp);

    if (flags & VERBOSE_OPT) {
        for (i = 0; i < (int)count; ++i)
            printf("%u\t", len_info[i]);
        printf("\n");
    }

    for (i = 0; i < (int)count && cal_width < width; ++i)
        cal_width += len_info[i] + 2;

    cal_col_count = i; // set cal_col_count to max possible column count

    // set width of each column
    info.col_len = (size_t*) malloc(sizeof(size_t) * i);
    for (i = 0; i < cal_col_count; ++i) {
        int rows = ceil((double) count / (double) cal_col_count);
        unsigned int max_width = 0;
        for (j = 0; j < rows; ++j) {
            if (entries[j*cal_col_count+i]->name_len + 2 > max_width)
                max_width = COLMAJ_IND(entries, cal_col_count, i, j)->name_len + 2;
        }
        info.col_len[i] = max_width;
    }

    // TODO: must return a pointer
    return info;
}

void clear_col_info(col_info base) {
    free(base.col_len);
}

int get_terminal_width(void) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}
