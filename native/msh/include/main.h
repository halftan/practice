#ifndef MAIN_H
#define MAIN_H 1

#include "arguments.h"

#define CWDLEN 200
char *cwd;
char **environ;

int  stateno;

#define EXIT_STATE   0x00000001
#define IF_STATE     0x00000002
#define SCRIPT_STATE 0x00000003

#define CHECK_STATE  0x0000000F
#define NORMAL       0

#define BUFSIZE      200
#define ARGBUFSIZE   50

void init(int argc, char *argv[]);
void sigint_handler(int sig);
void parse_line(char*, arguments*);
void print_prompt(void);
int exec_command(arguments *arg, int argc, char *argv[]);
int shell_builtin(arguments *arg, int argc, char *argv[]);
int exec_if_command(arguments *arg, int argc, char *argv[]);

#endif
