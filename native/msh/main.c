#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

#include "main.h"
#include "arguments.h"

int main(int argc, char *argv[], char *envp[]) {
    char c = 0;
    char *buf = (char*) malloc(sizeof(char) * BUFSIZE);
    arguments args;
    size_t buf_i;

    init_args(&args);

    init(argc, argv, envp);
    bzero(buf, sizeof(char) * BUFSIZE);
    print_prompt();
    for (c = getchar(), buf_i = 0;
            c != EOF; c = getchar()) {
        if (c == '\n') {
            parse_line(buf, &args);
            exec_command(&args, argc, argv, envp);
            clear_arg(&args);
            print_prompt();
            bzero(buf, sizeof(char) * BUFSIZE);
            buf_i = 0;
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


void print_prompt() {
    printf("[%s]$ ", cwd);
}

void exec_command(arguments *arg, int argc, char *argv[], char *envp[]) {
    pid_t pid;
    /* print_arg(arg); */
    if (arg->argc >= 1) {
        if ((pid = fork()) < 0) {
            fprintf(stderr, "Error: fork error.\n");
        } else if (pid == 0) {
            if (execvp(arg->argv[0], arg->argv) < 0) {
                fprintf(stderr, "%s: %s\n", arg->argv[0], strerror(errno));
                exit(-1);
            }
        } else {
            wait(NULL);
        }
    }
}

void init(int argc, char *argv[], char *envp[]) {
    cwd = (char *) malloc(sizeof(char) * CWDLEN);
    getcwd(cwd, CWDLEN);
    signal(SIGINT, sigint_handler);
}

void sigint_handler(int sig) {
    putchar('\n');
    print_prompt();
    fflush(stdout);
}
