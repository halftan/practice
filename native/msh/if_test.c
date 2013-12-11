#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/errno.h>

int main(int argc, char *argv[]) {
    int ret = 0;
    int stat = 0;
    pid_t pid;
    if ((pid = fork()) == 0) {
        ret = execvp("test", argv);
        if (ret == -1) {
            exit(errno);
        } else
            exit(0);
    } else {
        printf("The new pid is %d\n", pid);
        wait(&stat);
        stat = WEXITSTATUS(stat);
        switch (stat) {
            case 0:
                printf("True\n");
                break;
            case 1:
                printf("False\n");
                break;
            case 2:
                printf("Error\n");
                break;
            default:
                printf("Unknown exit code: %d\n", stat);
        }
    }
    return 0;
}
