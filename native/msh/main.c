#include "main.h"
#include "arguments.h"
#include "exec_if.h"
#include "err_handler.h"
#include "util.h"

int main(int argc, char *argv[], char *envp[]) {
    char *cmd = NULL;
    char *buf = (char*) malloc(sizeof(char) * BUFSIZE);
    arguments args;
    int ret;
    environ = envp;
    flags = 0;
    scriptf = NULL;

    init(argc, argv);
    init_args(&args);
    sprintf(buf, "[%s]$ ", cwd);

    if (argc > 1) {
        if ((scriptf = fopen(argv[1], "r")) == NULL)
            exit_error(errno, FILE_ERROR);
        else {
            flags |= SUPPROP;
        }
    }

    // Use readline
    while (stateno != EXIT_STATE
            && (cmd = msh_readline(buf)) != NULL) {
        parse_line(cmd, &args);
        ret = exec_command(&args);
        if (ret != NORMAL) {
            switch (ret) {
                case CHECK_STATE:
                    switch (stateno) {
                        case EXIT_STATE:
                            printf("Bye~\n");
                            break;
                        case IF_STATE:
                            proc_if_command(&args);
                            break;
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

int shell_builtin(arguments *arg) {
    if (arg->argc == 0)
        return 0;
    if (strcmp("exit", arg->argv[0]) == 0) {
        stateno = EXIT_STATE;
        return 1;
    } else if (strcmp("if", arg->argv[0]) == 0) {
        stateno = IF_STATE;
        return 1;
    } else if (strcmp("cd", arg->argv[0]) == 0) {
        if (arg->argc != 2)
            return 0;
        else
            return msh_chdir(arg->argv[1]);
    }
    return 0;
}

int exec_command(arguments *arg) {
    if (shell_builtin(arg))
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

void init(int argc, char *argv[]) {
    cwd = (char *) malloc(sizeof(char) * CWDLEN);
    getcwd(cwd, CWDLEN);
    signal(SIGINT, sigint_handler);
}

void sigint_handler(int sig) {
    putchar('\n');
    print_prompt();
    fflush(stdout);
}
