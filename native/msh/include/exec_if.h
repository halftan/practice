#ifndef EXEC_IF_H
#define EXEC_IF_H 1

#include "arguments.h"

typedef struct _if_arg {
  arguments condition;
  arguments then_command;
  arguments else_command;
} if_arg;

int exec_if(arguments *argchar, if_arg *args); 
int proc_if_command(arguments *arg);
int exec_then_command(arguments *arg);
int check_status(arguments*, int*, int);
void parse_if(arguments*, if_arg*);
void if_arg_init(if_arg*);
void merge_lines(arguments*, const char*);
void if_arg_clear(if_arg*);

#define STATUS_OTHER 0
#define STATUS_IF    1
#define STATUS_THEN  2
#define STATUS_ELSE  3
#define STATUS_FI    4
#endif
