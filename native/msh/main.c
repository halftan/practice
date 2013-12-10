#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "main.h"
#include "arguments.h"

int main(int argc, char *argv[], char *envp[]) {
    char *cmd = NULL;
    char *buf = (char*) malloc(sizeof(char) * BUFSIZE);
    arguments args;

    init(argc, argv, envp);
    init_args(&args);
    sprintf(buf, "[%s]$ ", cwd);

    // Use readline
    while ((cmd = readline(buf)) != NULL) {
        parse_line(cmd, &args);
        exec_command(&args, argc, argv, envp);
        add_history(cmd); // add to readline history
        clear_arg(&args);
        free(cmd);        // manually free readline returned buffer
        cmd = NULL;
    }
    del_arg(&args);

    free(cwd);
    free(buf);
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

int exec_command(arguments *arg, int argc, char *argv[], char *envp[]) {
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
    return 0;
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
