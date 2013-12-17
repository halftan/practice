#include "main.h"
#include "util.h"

int msh_chdir(const char *path) {
    chdir(path);
    getcwd(cwd, CWDLEN);
    return 0;
}

char *msh_readline(const char *prompt) {
    ssize_t len = 0;
    size_t t = 0;
    if (flags & SUPPROP) {
        char *line = NULL;
        len = getline(&line, &t, scriptf);
        if (len < 0)
            return NULL;
        if (line[len - 1] == '\n')
            line[len - 1] = 0;
        else if (len < (ssize_t)t)
            line[len] = 0;
        return line;
    }
    else
        return readline(prompt);
}
