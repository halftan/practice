#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/wait.h>

#include "main.h"
#include "util.h"
#include "arguments.h"
#include "exec_if.h"

int proc_if_command(arguments *arg)
{
  if_arg args;
  if_arg_init(&args);

  parse_if(arg, &args);

  exec_if(arg, &args);
 //int i;
 //printf("Condition:\n");
 //for(i = 0; i < args.condition.argc; i++)
 //{
 //  printf("%s\n", args.condition.argv[i]);
 //}
 //printf("Then:\n");
 //for(i = 0; i < args.then_command.argc; i++)
 //{
 //  printf("%s\n", args.then_command.argv[i]);
 //}
 //printf("Else:\n");
 //for(i = 0; i < args.else_command.argc; i++)
 //{
 //  printf("%s\n", args.else_command.argv[i]);
 //}
  return 0;
}

int exec_if(arguments *arg, if_arg *args)
{
  int ret, stat;
  pid_t pid;

  if ((pid = fork()) == 0)
  {
    if (args->condition.argc == 1)
      exit(2);
    ret = execvp(args->condition.argv[0], args->condition.argv);
    if(ret == -1)
    {
      exit(errno);
    }
    else
    {
      exit(0);
    }
  }
  else
  {
    wait(&stat);
    stat = WEXITSTATUS(stat);
    switch (stat)
    {
      case 0:
        exec_then_command(&args->then_command);
        break;
      case 1:
        exec_then_command(&args->else_command);
        break;
      case 2:
        printf("ERROR\n");
        break;
      default:
        printf("Unknown exit code %d\n", stat);
    }
  }

  return 0;
}

void parse_if(arguments *arg, if_arg *args)
{
  int i;
  int status = STATUS_OTHER;
  if (strcmp("fi", arg->argv[arg->argc-1]) != 0)
    merge_lines(arg, "fi");

  /* int test_i; */
  /* for (test_i = 0; test_i < arg->argc; test_i++) */
  /* { */
  /*   printf("%s", arg->argv[test_i]);  */
  /* } */

  for(i = 0; i < arg->argc; i++)
  {
    status = check_status(arg, &i, status);
    switch(status)
    {
      case STATUS_IF:
        add_arg(&args->condition, arg->argv[i]);
        break;
      case STATUS_THEN:
        add_arg(&args->then_command, arg->argv[i]);
        break;
      case STATUS_ELSE:
        add_arg(&args->else_command, arg->argv[i]);
        break;
      case STATUS_FI:
        break;
    }
  }
}

void if_arg_init(if_arg *args)
{
  init_args(&args->condition);
  init_args(&args->then_command);
  init_args(&args->else_command);
}

int check_status(arguments *arg, int *ind, int status)
{
  /* printf("%s\n", arg->argv[1]); */
  if (strcmp("if", arg->argv[*ind]) == 0)
  {
    ++*ind;
    return STATUS_IF;
  }
  else if (strcmp("then", arg->argv[*ind]) == 0)
  {
    ++*ind;
    return STATUS_THEN;
  }
  else if (strcmp("else", arg->argv[*ind]) == 0)
  {
    ++*ind;
    return STATUS_ELSE;
  }
  else if (strcmp("fi", arg->argv[*ind]) == 0)
  {
    ++*ind;
    return STATUS_FI;
  }
  else
  {
    return status;
  }
}

void merge_lines(arguments *arg, const char *word)
{
  char *line;

  do {
    line = msh_readline("> ");
    if (line == NULL)
        break;
    parse_line(line, arg);
    free(line);
  } while (strcmp(word, arg->argv[arg->argc-1]) != 0);
}

int exec_then_command(arguments *arg)
{
  pid_t pid;
  if ((pid = fork()) == 0)
  {
    if (execvp(arg->argv[0], arg->argv) < 0)
    {
      fprintf(stderr, "%s: %s\n", arg->argv[0], strerror(errno));
      exit(-1);
    }
    exit(0);
  }
  else
  {
    wait(NULL);
  }
  return 0;
}

void if_arg_clear(if_arg *args)
{
  clear_arg(&args->condition);
  clear_arg(&args->then_command);
  clear_arg(&args->else_command);
}

