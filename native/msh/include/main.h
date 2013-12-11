#ifndef MAIN_H
#define MAIN_H 1

#include "arguments.h"

#define CWDLEN 200
char *cwd;

int stateno;

#define EXIT_STATE 0x00000001
#define IF_STATE 0x00000002

#define CHECK_STATE 0x0000000F
#define NORMAL 0

#define BUFSIZE 200
#define ARGBUFSIZE 50

void init(int argc, char *argv[], char *envp[]);
void sigint_handler(int sig);
void parse_line(char*, arguments*);
void print_prompt(void);
int exec_command(arguments *arg, int argc, char *argv[], char *envp[]);
int shell_builtin(arguments *arg, int argc, char *argv[], char *envp[]);
int exec_if_command(arguments *arg, int argc, char *argv[], char *envp[]);

#endif
