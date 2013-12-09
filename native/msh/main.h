#ifndef MAIN_H
#define MAIN_H 1

typedef struct _arguments {
    char **argv;
    int argc;
    int _alloc;
} arguments;

arguments *init_args(arguments*);
arguments *add_arg(arguments*, char*);
void del_arg(arguments*);
void clear_arg(arguments*);
void _arg_alloc(arguments*);
void _print_arg(arguments*);

#define INIT_ARGC 5
#define STEP_ARGC 5

#define CWDLEN 200
char *cwd;

#define BUFSIZE 500
#define ARGBUFSIZE 50

int FSM_CUR;

void init(int argc, char *argv[], char *envp[]);
void parse_line(char*, arguments*);
void print_prompt(void);
void exec_command(arguments *arg, int argc, char *argv[], char *envp[]);

#endif
