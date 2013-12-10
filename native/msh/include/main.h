#ifndef MAIN_H
#define MAIN_H 1

#include "arguments.h"

#define CWDLEN 200
char *cwd;

#define BUFSIZE 200
#define ARGBUFSIZE 50

void init(int argc, char *argv[], char *envp[]);
void sigint_handler(int sig);
void parse_line(char*, arguments*);
void print_prompt(void);
int exec_command(arguments *arg, int argc, char *argv[], char *envp[]);

#endif
