#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "arguments.h"

arguments *init_args(arguments *arg) {
    int i;
    arg->argc = 0;
    arg->_alloc = INIT_ARGC;
    arg->argv = (char**) malloc(sizeof(char*) * arg->_alloc);
    for (i = 0; i < arg->_alloc; ++i)
        arg->argv[i] = NULL;
    return arg;
}

arguments *add_arg(arguments *arg, char *str) {
    if (arg->_alloc <= arg->argc + 1)
        _arg_alloc(arg);
    arg->argv[arg->argc++] = strdup(str);

    return arg;
}

void parse_line(char *buf, arguments *arg) {
    int i, arg_i;
    char *argbuf = (char*) malloc(sizeof(char) * ARGBUFSIZE);

    bzero(argbuf, sizeof(char) * ARGBUFSIZE);
    for (i = 0, arg_i = 0;
            buf[i]; ++i) {
        if (buf[i] == ' ') {
            add_arg(arg, argbuf);
            arg_i = 0;
            bzero(argbuf, sizeof(char) * ARGBUFSIZE);
        } else {
            argbuf[arg_i++] = buf[i];
        }
    }
    if (argbuf[0])
        add_arg(arg, argbuf);
}

static void _arg_alloc(arguments *arg) {
    int i;
    arg->_alloc += STEP_ARGC;
    arg->argv = (char**) realloc(arg->argv, sizeof(char*) * arg->_alloc);
    for (i = arg->argc; i < arg->_alloc; ++i) {
        arg->argv[i] = NULL;
    }
}

void del_arg(arguments *arg) {
    int i;
    for (i = 0; i < arg->_alloc; ++i) {
        free(arg->argv[i]);
        arg->argv[i] = NULL;
    }
    free(arg->argv);
    arg->argv = NULL;
}

void clear_arg(arguments *arg) {
    int i;
    for (i = 0; i < arg->argc; ++i) {
        free(arg->argv[i]);
        arg->argv[i] = NULL;
    }
    arg->argc = 0;
    /* printf("Cleared arguments\n"); */
}

void print_arg(arguments *arg) {
    int i;
    for (i = 0; i < arg->argc; ++i) {
        printf("%d: %s\n", i, arg->argv[i]);
    }
}
