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
#include <pwd.h>
#include <grp.h>
#include <time.h>

#ifndef NAME_MAX
#define NAME_MAX 256
#endif

#define LEN_MAX  1000

#define OPT_STRING "lavrchR"

/***************
 *  EXIT CODE  *
 ***************/
#define FAILURE_VAL 2
#define SUCCESS_VAL 0

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
static     void print_usage();
static     void ls_print(entry_stat **entries, size_t file_count);
static      int get_terminal_width();
static     void copy_entry(entry_stat **entries, struct dirent *direntp, size_t file_count);
static     void free_entry(entry_stat **entries, size_t file_count);
static col_info *calc_col_info(entry_stat **entries, size_t count, col_info *buf);
static     void show_file_info(entry_stat *entry);

// used for qsort
static      int dname_cmp(const void *, const void *);
static      int namlen_cmp(const void *p1, const void *p2);
static     char *mode_to_string(int mode, char str[]);
static     char *uid_to_name(uid_t uid);
static     char *gid_to_name(gid_t gid);

int main(int argc, char *argv[])
{
    int opt;
    int i;

    flags = 0;
    cwd = getcwd(NULL, LEN_MAX);
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
            case 'h':
                print_usage();
                exit(SUCCESS_VAL);
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

void do_ls(char *dirname) {
    DIR *dir_ptr;
    struct dirent *direntp;
    entry_stat **entries;
    char **dirs = NULL;
    size_t i;
    size_t dir_count, file_count, st_allocated;
    char orig_dir[LEN_MAX];

    file_count = 0;
    dir_count = 0;
    st_allocated = 20;
    entries = (entry_stat**) malloc(sizeof(entry_stat*) * st_allocated);
    getcwd(orig_dir, LEN_MAX);

    if (flags & VERBOSE_OPT)
        printf("Original dir:\t%s\n", orig_dir);

    if ((dir_ptr = opendir(dirname)) == NULL) {
        fprintf(stderr, "%s: cannot open %s\n", my_name, dirname);
    } else {
        // chdir to destination directory
        chdir(dirname);

        while ( (direntp = readdir(dir_ptr)) != NULL ) {
            if (flags & ALL_OPT && direntp->d_name[0] == '.')
                copy_entry(entries, direntp, file_count++);
            else if (direntp->d_name[0] != '.')
                copy_entry(entries, direntp, file_count++);
            if (st_allocated == file_count) {
                st_allocated += 20;
                entries = (entry_stat**) realloc(entries,
                                                 sizeof(entry_stat*) * st_allocated);
            }
        }

        if (flags & VERBOSE_OPT) {
            printf("Verbose info\n");
            printf("dir count: %zu, entries allocated: %zu\n", file_count, st_allocated);
            for (i = 0; i < file_count; ++i)
                printf("%s\n", entries[i]->f_name);
            printf("Verbose info end.\n\n");
        }

        // sort by dirnames in alphabetical order
        qsort(entries, file_count, sizeof(entry_stat*), dname_cmp);

        closedir(dir_ptr);

        if (flags & MULTI_DIR_OPT)
            printf("\n%s:\n", dirname);
        ls_print(entries, file_count);

        if (flags & RECURSIVE_OPT) {
            dirs = (char**) malloc(sizeof(char*) * file_count);
            for (i = 0; i < file_count; ++i) {
                if (S_ISDIR(entries[i]->f_stat.st_mode)) {
                    dirs[dir_count] = (char*) malloc(sizeof(char) * LEN_MAX);
                    strncpy(dirs[dir_count], dirname, LEN_MAX);
                    strncat(dirs[dir_count], "/", LEN_MAX);
                    strncat(dirs[dir_count], entries[i]->f_name, LEN_MAX);
                    dir_count++;
                }
            }
        }
        free_entry(entries, file_count);
        entries = NULL;
        // chdir back to original directory.
        if (orig_dir != NULL)
            chdir(orig_dir);
        if (flags & VERBOSE_OPT) {
            printf("Dirs to be recursively lsed:\n");
            for (i = 0; i < dir_count; ++i)
                printf("%s\n", dirs[i]);
        }
        for (i = 0; i < dir_count; ++i) {
            do_ls(dirs[i]);
        }
        for (i = 0; i < dir_count; ++i)
            free(dirs[i]);
        if (dirs)
            free(dirs);

    }

    if (entries)
        free(entries);
}

void ls_print(entry_stat **entries, size_t file_count) {
    int i, t;
    col_info colinfo;
    char **pstr;

    if (file_count == 0)
        return;

    if (flags & LIST_OPT) {
        printf("File count: %zu\n", file_count);
        if (flags & REVERSE_OPT) {
            for (i = (int)file_count - 1; i >= 0; --i)
                show_file_info(entries[i]);
        } else
            for (i = 0; i < (int)file_count; ++i)
                show_file_info(entries[i]);
    } else {
        calc_col_info(entries, file_count, &colinfo);
        pstr = (char**) malloc(sizeof(char*) * colinfo.col_count);
        for (i = 0; i < (int)colinfo.col_count; ++i) {
            pstr[i] = (char*) malloc(sizeof(char) * NAME_MAX);
            sprintf(pstr[i], "%%-%zus", colinfo.col_len[i]);
        }

        if (flags & VERBOSE_OPT)
            for (i = 0; i < (int)colinfo.col_count; ++i)
                printf("%s\n", pstr[i]);

        if (flags & REVERSE_OPT) {
            for (t = 0, i = (int)file_count - 1; i >= 0; --i) {
                printf(pstr[i % colinfo.col_count], entries[i]->f_name);
                if (++t == (int)colinfo.col_count) {
                    t = 0;
                    putchar('\n');
                }
            }
        } else {
            for (t = 0, i = 0; i < (int)file_count; ++i) {
                printf(pstr[i % colinfo.col_count], entries[i]->f_name);
                if (++t == (int)colinfo.col_count) {
                    t = 0;
                    putchar('\n');
                }
            }
        }

        if (file_count % colinfo.col_count)
            putchar('\n');

        free(colinfo.col_len);
        for (i = 0; i < (int)colinfo.col_count; ++i)
            free(pstr[i]);
        if (pstr)
            free(pstr);
    }
}

void copy_entry(entry_stat **entries, struct dirent *direntp, size_t file_count) {
    // allocate and clear memory
    entries[file_count] = (entry_stat*) malloc(sizeof(entry_stat));
    memset(entries[file_count], 0, sizeof(entry_stat));
    // set data in struct
    entries[file_count]->f_name = strndup(direntp->d_name, NAME_MAX);
    entries[file_count]->name_len = strlen(direntp->d_name);
    stat(direntp->d_name, &entries[file_count]->f_stat);
}

void free_entry(entry_stat **entries, size_t file_count) {
    size_t i;
    for (i = 0; i < file_count; ++i) {
        free(entries[i]->f_name);
        free(entries[i]);
    }
    free(entries);
}

int dname_cmp(const void *p1, const void *p2) {
    const entry_stat *a = *(const entry_stat**)p1;
    const entry_stat *b = *(const entry_stat**)p2;
    if (flags & IGCASE_OPT)
        return strcmp(a->f_name, b->f_name);
    else
        return strcasecmp(a->f_name, b->f_name);
}

int namlen_cmp(const void *p1, const void *p2) {
    return *(unsigned int *)p2 - *(unsigned int *)p1;
}

col_info *calc_col_info(entry_stat **entries, size_t count, col_info *buf) {
    int i, j, rows, width, cal_width, cal_col_count;
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

    // set cal_col_count to max possible column count
    cal_col_count = i > 1 ? i - 1 : 1;

    // set width of each column
    buf->col_len = (size_t*) malloc(sizeof(size_t) * i);

    rows = ceil((double) count / (double) cal_col_count);
    for (i = 0; i < cal_col_count; ++i) {
        unsigned int max_width = 0;
        for (j = 0; j < rows; ++j) {
            if (j*cal_col_count+i < (int)count
                    && entries[j*cal_col_count+i]->name_len + 2 > max_width)
                max_width = COLMAJ_IND(entries, cal_col_count, i, j)->name_len + 2;
        }
        buf->col_len[i] = max_width;
    }

    buf->col_count = cal_col_count;
    free(len_info);

    return buf;
}

int get_terminal_width(void) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    if (w.ws_col == 0)
        return 80;
    else
        return w.ws_col;
}

void show_file_info(entry_stat *entry) {
    char modestr[11];

    mode_to_string(entry->f_stat.st_mode, modestr);

    printf("%s", modestr);
    printf("%4d ", (int) entry->f_stat.st_nlink);
    printf("%-8s ", uid_to_name(entry->f_stat.st_uid));
    printf("%-8s ", gid_to_name(entry->f_stat.st_gid));
    printf("%8ld ", (long)entry->f_stat.st_size);
    printf("%.12s ", 4+ctime(&entry->f_stat.st_mtime));
    printf("%s\n", entry->f_name);
}

char *mode_to_string(int mode, char str[]) {
    strcpy(str, "----------");

    // dir? char dev? blk dev?
    if (S_ISDIR(mode)) str[0] = 'd';
    if (S_ISCHR(mode)) str[0] = 'c';
    if (S_ISBLK(mode)) str[0] = 'b';

    if (mode & S_IRUSR) str[1] = 'r';
    if (mode & S_IWUSR) str[2] = 'w';
    if (mode & S_IXUSR) str[3] = 'x';

    if (mode & S_IRGRP) str[4] = 'r';
    if (mode & S_IWGRP) str[5] = 'w';
    if (mode & S_IXGRP) str[6] = 'x';

    if (mode & S_IROTH) str[7] = 'r';
    if (mode & S_IWOTH) str[8] = 'w';
    if (mode & S_IXOTH) str[9] = 'x';

    return str;
}

char *uid_to_name(uid_t uid) {
    struct passwd *pwp;
    static char buf[20];
    if ( (pwp = getpwuid(uid)) == NULL ) {
        sprintf(buf, "%d", uid);
        return buf;
    } else
        return pwp->pw_name;
}
char *gid_to_name(gid_t gid) {
    struct group *grpp;
    static char buf[20];
    if ( (grpp = getgrgid(gid)) == NULL ) {
        sprintf(buf, "%d", gid);
        return buf;
    } else
        return grpp->gr_name;
}

void print_usage() {
    fprintf(stderr, "Usage: %s [-%s]\n\
    [-l]\t\tDisplay long info.\n\
    [-a]\t\tDisplay all files.\n\
    [-v]\t\tVerbose mode.\n\
    [-r]\t\tReverse sort.\n\
    [-c]\t\tCase SeNsiTivE sort.\n\
    [-R]\t\tRecursive list.\n\
    [-h]\t\tThis help message.\n",
            my_name, OPT_STRING);   // lavrchR
}
