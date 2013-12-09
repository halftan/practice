#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "main.h"

int main(int argc, char *argv[], char *envp[]) {
    char c = 0;
    char *buf = (char*) malloc(sizeof(char) * BUFSIZE);
    arguments args;
    size_t buf_i;

    init_args(&args);
    cwd = (char *) malloc(sizeof(char) * CWDLEN);
    getcwd(cwd, CWDLEN);
    print_prompt();

    bzero(buf, sizeof(char) * BUFSIZE);
    for (c = getchar(), buf_i = 0;
            c != EOF; c = getchar()) {
        if (c == '\n') {
            parse_line(buf, &args);
            exec_command(&args, argc, argv, envp);
            clear_arg(&args);
            print_prompt();
        } else {
            buf[buf_i++] = c;
        }
    }
    del_arg(&args);
    return 0;
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

void _arg_alloc(arguments *arg) {
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
    printf("Cleared arguments\n");
}

void print_prompt() {
    printf("[%s]$ ", cwd);
}

void exec_command(arguments *arg, int argc, char *argv[], char *envp[]) {
    _print_arg(arg);
    if (arg->argc >= 1 && strcmp("ls", arg->argv[0]) == 0) {
        execve("/bin/ls", arg->argv, envp);
    }
}

void _print_arg(arguments *arg) {
    int i;
    for (i = 0; i < arg->argc; ++i) {
        printf("%d: %s\n", i, arg->argv[i]);
    }
}
