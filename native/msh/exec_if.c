#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/wait.h>

#include "arguments.h"
#include "exec_if.h"

int exec_if_command(arguments *arg, int argc, char *argv[], char *envp[])
{
  int ret, stat;
  pid_t pid;
  if_arg args;
  if_arg_init(&args);

  parse_if(arg, &args);

  if ((pid = fork()) == 0)
  {
    int i;
    for(i = 0; i < arg->argc; i++)
      printf("%s", args.condition.argv[i]);
    ret = execvp("text", args.condition.argv);
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
        exec_then_command(&args.then_command);
        break;
      case 1:
        //exec_else_command();
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
  int i, if_status;
  for(i = 0, if_status = 0; i < arg->argc; i++)
  {
    if_status = check_status(arg->argv[i], if_status);
    if(if_status == STATUS_IF)
    {
      add_arg(&args->condition, arg->argv[i]);
    }
    else if(if_status == STATUS_THEN)
    {
      add_arg(&args->then_command, arg->argv[i]);
    }
    else if(if_status == STATUS_ELSE)
    {
      add_arg(&args->else_command, arg->argv[i]);
    }
    
  }
}

void if_arg_init(if_arg *args)
{
  init_args(&args->condition);
  init_args(&args->then_command);
  init_args(&args->else_command);
}

int check_status(char *word, int status)
{
  if (strcmp("if", word) == 0)
  {
    return STATUS_IF;
  }
  else if (strcmp("then", word) == 0)
  {
    return STATUS_THEN;
  }
  else if (strcmp("else", word) == 0)
  {
    return STATUS_ELSE;
  }
  else if (strcmp("fi", word) == 0)
  {
    return STATUS_FI;
  }
  else
  {
    return status;
  }
}

int exec_then_command(arguments *arg)
{
  pid_t pid;
  if (arg->argc >= 1)
  {
    if ((pid = fork()) == 0)
    {
      if (execvp(arg->argv[0], arg->argv) < 0)
      {
        fprintf(stderr, "%s: %s\n", arg->argv[0], strerror(errno));
        exit(-1);
      }
    }
    else
    {
      wait(NULL);
    }
  }
  return 0;
}
