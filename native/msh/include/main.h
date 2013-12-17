#ifndef MAIN_H
#define MAIN_H 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include <readline/readline.h>
#include <readline/history.h>


#include "arguments.h"

#define CWDLEN 200

typedef unsigned int uint;

char *cwd;
char **environ;

int  stateno;
uint flags;

FILE *scriptf;

#define SUPPROP      0x00000001

#define EXIT_STATE   0x00000001
#define IF_STATE     0x00000002
#define CD_STATE     0x00000003
#define SCRIPT_STATE 0x00000004

#define CHECK_STATE  0x0000000F
#define NORMAL       0

#define BUFSIZE      200
#define ARGBUFSIZE   50

#define FILE_ERROR 4

void init(int argc, char *argv[]);
void sigint_handler(int sig);
void parse_line(char*, arguments*);
void print_prompt(void);
int exec_command(arguments *arg);
int shell_builtin(arguments *arg);
int exec_if_command(arguments *arg);

#endif
