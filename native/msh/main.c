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
#include "exec_if.h"

int main(int argc, char *argv[], char *envp[]) {
    char *cmd = NULL;
    char *buf = (char*) malloc(sizeof(char) * BUFSIZE);
    arguments args;
    int ret;

    init(argc, argv, envp);
    init_args(&args);
    sprintf(buf, "[%s]$ ", cwd);

    // Use readline
    while (stateno != EXIT_STATE
            && (cmd = readline(buf)) != NULL) {
        parse_line(cmd, &args);
        ret = exec_command(&args, argc, argv, envp);
        if (ret != NORMAL) {
            switch (ret) {
                case CHECK_STATE:
                    switch (stateno) {
                        case EXIT_STATE:
                            printf("Bye~\n");
                            break;
                        case IF_STATE:
                            proc_if_command(&args, argc, argv, envp);
                            break;
                        case SCRIPT_STATE:
                            proc_script_command(&args, argc, argv, envp);
                    }
                    break;
            }
        }
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

void print_prompt() {
    printf("[%s]$ ", cwd);
}

int shell_builtin(arguments *arg, int argc, char *argv[], char *envp[]) {
    if (strcmp("exit", arg->argv[0]) == 0) {
        stateno = EXIT_STATE;
        return 1;
    }
    else if (strcmp("if", arg->argv[0]) == 0)
    {
        stateno = IF_STATE;
        return 1;
    }
    else if (strcmp("msh", arg->argv[0]) == 0)
    {
        stateno = SCRIPT_STATE;
        return 1;
    }
    return 0;
}

int exec_command(arguments *arg, int argc, char *argv[], char *envp[]) {
    if (shell_builtin(arg, argc, argv, envp))
        return CHECK_STATE;
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
    return NORMAL;
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
