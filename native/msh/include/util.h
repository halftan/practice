#ifndef UTIL_H
#define UTIL_H 1
#include <unistd.h>
#include <readline/readline.h>

int msh_chdir(const char *);
char *msh_readline(const char *prompt);
#endif
