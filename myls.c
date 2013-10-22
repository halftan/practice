/**
 * Author: HalFtaN
 *
 **/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#define OPT_STRING "lavr"

/***************
 *  EXIT CODE  *
 ***************/
#define FAILURE_VAL 2

#define LIST_OPT 0x0001
#define ALL_OPT 0x0100
#define RECURSIVE_OPT 0x0010

void do_ls(char *dir, int flags);
void print_usage(char *argv[]);

int main(int argc, char *argv[])
{
    int opt, verbose;
    int flags, optcount;
    char *cwd[200];

    flags = 0;
    verbose = 0;
    optcount = 0;

    while ((opt = getopt(argc, argv, OPT_STRING)) != -1) {
        optcount++;
        switch (opt) {
        case 'v':
            verbose = 1;
            break;
        case 'l':
            flags |= LIST_OPT;
            break;
        case 'r':
            flags |= RECURSIVE_OPT;
            break;
        case 'a':
            flags |= ALL_OPT;
            break;
        default:
            print_usage(argv);
            exit(FAILURE_VAL);
        }
    }

    if (argc == 1) {
        do_ls(".", flags);
    } else {
        while (--argc)
            do_ls(*++argv, flags);
    }

    return 0;
}

void print_usage(char *argv[]) {
    fprintf(stderr, "Usage: %s [-l] [-v] [-r]\n",
            argv[0]);
}

void do_ls(char *dirname, int flags) {
    DIR *dir_ptr;
    struct dirent *direntp;
    char entries[200][200];
    int dir_count, i;

    dir_count = 0;
    if ((dir_ptr = opendir(dirname)) == NULL) {
        fprintf(stderr, "myls: cannot open %s\n", dirname);
    } else {
        while ( (direntp = readdir(dir_ptr)) != NULL ) {
            if (flags & ALL_OPT && direntp->d_name[0] == '.')
                strcpy(entries[dir_count++], direntp->d_name);
            else if (direntp->d_name[0] != '.')
                strcpy(entries[dir_count++], direntp->d_name);
        }
        closedir(dir_ptr);
        for (i = 0; i < dir_count; ++i) {
            printf("%s\n", entries[i]);
        }
    }
}
