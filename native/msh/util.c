#include "main.h"
#include "util.h"

int msh_chdir(const char *path) {
    chdir(path);
    getcwd(cwd, CWDLEN);
    return 0;
}

char *msh_readline(const char *prompt) {
    if (flags & SUPPROP)
        return readline(NULL);
    else
        return readline(prompt);
}
